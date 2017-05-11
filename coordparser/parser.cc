//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/parser.h"

#include <utility>
#include <vector>

#include "coordparser/feature.h"
#include "coordparser/logger.h"

namespace coordparser {

Parser::Parser(std::shared_ptr<Classifier> classifier) :
    classifier_(classifier) {}

GreedyParser::GreedyParser(std::shared_ptr<Classifier> classifier) :
    Parser(classifier) {}

std::shared_ptr<State> GreedyParser::parse(const Sentence& sentence) {
  std::shared_ptr<State> state = std::make_shared<State>(&sentence);
  while (!state->isTerminal()) {
    // retrieve an one best action greedily
    Transition::apply(getNextAction(*state), state);
  }
  return state;
}

Action GreedyParser::getNextAction(const State& state) {
  LOG_TRACE("{}", state);
  std::vector<float> scores =
      classifier_->compute(std::move(Feature::extract(state)));
  LOG_TRACE("scores: {}", scores);
  unsigned best_action = 0;
  float best_score = -INFINITY;
  for (unsigned i = 1; i < scores.size(); ++i) {
    if (scores[i] > best_score && Transition::isAllowed(i, state)) {
      best_action = i;
    }
  }
  LOG_TRACE("best action: {}", best_action);
  return static_cast<Action>(best_action);
}

}  // namespace coordparser
