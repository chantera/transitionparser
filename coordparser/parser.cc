//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/parser.h"

namespace coordparser {

Parser::Parser(std::shared_ptr<Classifier> classifier) :
    classifier_(classifier) {}

State GreedyParser::parse(const Sentence& sentence) {
  State state(sentence);
  while (!state.isTerminal()) {
    // retrieve an one best action greedily
    Action action = classifier_->getNextAction(state);
    // state = action.apply(state);
  }
  return state;
}

GreedyParser::GreedyParser(std::shared_ptr<Classifier> classifier) :
    Parser(classifier) {}

}  // namespace coordparser
