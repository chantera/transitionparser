//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/transition.h"

#include <utility>
#include <vector>

namespace coordparser {

int Transition::numActionTypes() {
  return 3;
}

int Transition::numActions(int num_labels) {
  return 1 + num_labels * 2;
}

// The SHIFT action uses the same value as the corresponding action type.
Action Transition::shiftAction() {
  return SHIFT;
}

// The LEFT action converts the label to an odd number greater or equal to 1.
Action Transition::leftAction(int label) {
  return LEFT + (label << 1);
}

// The RIGHT action converts the label to an even number greater or equal to 2.
Action Transition::rightAction(int label) {
  return RIGHT + (label << 1);
}

// Extracts the action type from a given parser action.
Transition::ActionType Transition::actionType(Action action) {
  return static_cast<ActionType>(action < 1 ? action : 1 + (~action & 1));
}

// Extracts the label from a given parser action. If the action is SHIFT,
// returns -1.
int Transition::label(Action action) {
  return action < 1 ? -1 : (action - 1) >> 1;
}

void Transition::apply(Action action, State* state) {
  switch (actionType(action)) {
    case SHIFT:
      shift(state);
      break;
    case LEFT:
      left(state, label(action));
      break;
    case RIGHT:
      right(state, label(action));
      break;
    default:
      COORDPARSER_EXCEPTION("INVALID ACTION {}", action);
      break;
  }
  state->record(action);
}

// Shift: (s, i|b, A) => (s|i, b, A)
void Transition::shift(State* state) {
  state->push(state->buffer());
  state->advance();
}

// Left: (s|i|j, b, A) => (s|j, b, A +(j,l,i))
void Transition::left(State* state, int label) {
  int s0 = state->pop();
  int s1 = state->pop();
  state->addArc(s1, s0, label);
  state->push(s0);
}

// Right: (s|i|j, b, A) => (s|i, b, A +(i,l,j))
void Transition::right(State* state, int label) {
  int s0 = state->pop();
  int s1 = state->top();
  state->addArc(s0, s1, label);
}

bool Transition::isAllowed(Action action, const State& state) {
  switch (actionType(action)) {
    case SHIFT:
      return isAllowedShift(state);
    case LEFT:
      return isAllowedLeft(state);
    case RIGHT:
      return isAllowedRight(state);
  }
  return false;
}

bool Transition::isAllowedShift(const State& state) {
  return !state.end();
}

bool Transition::isAllowedLeft(const State& state) {
  return state.stackSize() > 2;
}

bool Transition::isAllowedRight(const State& state) {
  return state.stackSize() > 1;
}

bool Transition::isTerminal(const State& state) {
  return state.end() && state.stackSize() < 2;
}

Action Transition::getOracle(const State& state) {
  if (state.stackSize() < 2) {
    // assert !state.isTerminal()
    return shiftAction();
  }
  const Token& s0 = state.getToken(state.stack(0));
  const Token& s1 = state.getToken(state.stack(1));
  if (s0.head == s1.id && doneRightChildrenOf(state, s0.id)) {
    return rightAction(s0.label);
  }
  if (s1.head == s0.id) {
    return leftAction(s1.label);
  }
  return shiftAction();
}

bool Transition::doneRightChildrenOf(const State& state, int head) {
  int index = state.buffer();
  while (index < state.numTokens()) {
    int actual_head = state.getToken(index).head;
    if (actual_head == head) return false;
    index = head > index ? head : index + 1;
  }
  return true;
}

}  // namespace coordparser
