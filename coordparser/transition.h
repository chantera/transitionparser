//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_TRANSITION_H_
#define COORDPARSER_TRANSITION_H_

#include <memory>

#include "coordparser/state.h"
#include "coordparser/utility.h"

namespace coordparser {

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

  static Action shiftAction();

  static Action leftAction(int label);

  static Action rightAction(int label);

  static ActionType actionType(Action action);

  static int label(Action action);

  static void apply(
      Action action,
      std::shared_ptr<State>& state);  // NOLINT(runtime/references)

  static State* shift(const State& state);

  static State* left(const State& state, int label);

  static State* right(const State& state, int label);

  static bool isAllowed(Action action, const State& state);

  static bool isAllowedShift(const State& state);

  static bool isAllowedLeft(const State& state);

  static bool isAllowedRight(const State& state);

  static Action getOracle(const State& state);

  static bool doneRightChildrenOf(const State& state, int head);

 private:
  Transition() = delete;
  DISALLOW_COPY_AND_MOVE(Transition);
  ~Transition() = delete;
};

}  // namespace coordparser

#endif  // COORDPARSER_TRANSITION_H_
