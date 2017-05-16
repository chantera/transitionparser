//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/transition.h"

#include <utility>
#include <vector>

namespace coordparser {

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

void Transition::apply(Action action, std::shared_ptr<State>& state) {
  switch (actionType(action)) {
    case SHIFT:
      state.reset(shift(*state));
      break;
    case LEFT:
      state.reset(left(*state, label(action)));
      break;
    case RIGHT:
      state.reset(right(*state, label(action)));
      break;
  }
}

// Shift: (s, i|b, A) => (s|i, b, A)
State* Transition::shift(const State& state) {
  std::vector<int> stack(state.stack_);
  stack.reserve(state.num_tokens_);
  stack.push_back(state.buffer_);
  return new State(state, shiftAction(), std::move(stack), state.buffer_ + 1,
                   state.heads_, state.labels_);
}

// Left: (s|i|j, b, A) => (s|j, b, A +(j,l,i))
State* Transition::left(const State& state, int label) {
  std::vector<int> stack(state.stack_);
  std::vector<int> heads(state.heads_);
  std::vector<int> labels(state.labels_);
  int s0 = stack.back();
  stack.pop_back();
  int s1 = stack.back();
  stack.pop_back();
  heads[s1] = s0;
  labels[s1] = label;
  stack.push_back(std::move(s0));
  return new State(state, leftAction(label), std::move(stack), state.buffer_,
                   std::move(heads), std::move(labels));
}

// Right: (s|i|j, b, A) => (s|i, b, A +(i,l,j))
State* Transition::right(const State& state, int label) {
  std::vector<int> stack(state.stack_);
  std::vector<int> heads(state.heads_);
  std::vector<int> labels(state.labels_);
  int s0 = stack.back();
  stack.pop_back();
  int s1 = stack.back();
  heads[s0] = s1;
  labels[s0] = label;
  return new State(state, rightAction(label), std::move(stack), state.buffer_,
                   std::move(heads), std::move(labels));
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
  return state.buffer_ < state.num_tokens_;
}

bool Transition::isAllowedLeft(const State& state) {
  return state.stack_.size() > 2;
}

bool Transition::isAllowedRight(const State& state) {
  return state.stack_.size() > 1;
}

Action Transition::getOracle(const State& state) {
  if (state.stack_.size() < 2) {
    // assert !state.isTerminal()
    return shiftAction();
  }
  const Token& s0 = state.getStackToken(0);
  const Token& s1 = state.getStackToken(1);
  if (s0.head == s1.id && doneRightChildrenOf(state, s0.id)) {
    return rightAction(s0.label);
  }
  if (s1.head == s0.id) {
    return leftAction(s1.label);
  }
  return shiftAction();
}

bool Transition::doneRightChildrenOf(const State& state, int head) {
  int index = state.buffer_;
  while (index < state.num_tokens_) {
    int actual_head = state.getToken(index).head;
    if (actual_head == head) return false;
    index = head > index ? head : index + 1;
  }
  return true;
}

}  // namespace coordparser
