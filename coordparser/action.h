//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_ACTION_H_
#define COORDPARSER_ACTION_H_

#include <memory>

#include "coordparser/state.h"

namespace coordparser {

class State;

class Action {
 public:
  enum {
      SHIFT  = 0,
      RIGHT  = 1,
      LEFT   = 2,
  };

  std::shared_ptr<State> apply(const std::shared_ptr<State>& state);
};

}  // namespace coordparser

#endif  // COORDPARSER_ACTION_H_
