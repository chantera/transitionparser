//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/parser.h"
#include "coordparser/logger.h"

namespace coordparser {

Parser::Parser(std::shared_ptr<Classifier> classifier) :
    classifier_(classifier) {}

std::shared_ptr<State> GreedyParser::parse(const Sentence& sentence) {
  std::shared_ptr<State> state = std::make_shared<State>(&sentence);
  Logger::debug("Parse: %s", sentence);
  while (!state->isTerminal()) {
    // retrieve an one best action greedily
    Action action = classifier_->getNextAction(*state);
    Transition::apply(action, state);
  }
  return state;
}

GreedyParser::GreedyParser(std::shared_ptr<Classifier> classifier) :
    Parser(classifier) {}

}  // namespace coordparser
