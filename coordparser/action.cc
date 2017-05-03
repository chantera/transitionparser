//
// Created by Hiroki Teranishi on 4/7/17.
//

#include "action.h"

namespace coordparser {

std::shared_ptr<State> Action::apply(const std::shared_ptr<State>& state) {
  //return std::make_shared<State>(nullptr);
  return std::shared_ptr<State>(nullptr);
}

}
