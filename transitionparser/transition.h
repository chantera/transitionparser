//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef TRANSITIONPARSER_TRANSITION_H_
#define TRANSITIONPARSER_TRANSITION_H_

#include <memory>

#include "transitionparser/state.h"
#include "transitionparser/utility.h"

namespace transitionparser {

class State;

typedef int Action;
static const Action NoneAction = -1;

class Transition {
 public:
  enum ActionType {
      SHIFT  = 0,
      LEFT   = 1,
      RIGHT  = 2,
  };

  static int numActionTypes();

  static int numActions(int num_labels);

  static Action shiftAction();

  static Action leftAction(int label);

  static Action rightAction(int label);

  static ActionType actionType(Action action);

  static int label(Action action);

  static void apply(Action action, State* state);

  static void shift(State* state);

  static void left(State* state, int label);

  static void right(State* state, int label);

  static bool isAllowed(Action action, const State& state);

  static bool isAllowedShift(const State& state);

  static bool isAllowedLeft(const State& state);

  static bool isAllowedRight(const State& state);

  static bool isTerminal(const State& state);

  static Action getOracle(const State& state);

  static bool doneRightChildrenOf(const State& state, int head);

 private:
  Transition() = delete;
  DISALLOW_COPY_AND_MOVE(Transition);
  ~Transition() = delete;
};

}  // namespace transitionparser

#endif  // TRANSITIONPARSER_TRANSITION_H_
