//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/action.h"

namespace coordparser {

std::shared_ptr<State> Action::apply(const std::shared_ptr<State>& state) {
  // return std::make_shared<State>(nullptr);
  return std::shared_ptr<State>(nullptr);
}

}  // namespace coordparser
