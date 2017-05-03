//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "parser.h"

namespace coordparser {

State GreedyParser::parse(const Sentence& sentence) {
  State state(sentence);
  while (!state.isTerminal()) {
    Action action = classifier_->getNextAction(state);  // retrieve an one best action greedily
    //state = action.apply(state);
  }
  return state;
}

}
