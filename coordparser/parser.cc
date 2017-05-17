//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/parser.h"

#include <dynet/expr.h>
#include <dynet/tensor.h>

#include <utility>
#include <vector>

#include "coordparser/feature.h"
#include "coordparser/logger.h"

namespace coordparser {

Parser::Parser(std::shared_ptr<Classifier> classifier) :
    classifier_(classifier) {}

GreedyParser::GreedyParser(std::shared_ptr<Classifier> classifier) :
    Parser(classifier) {}

std::unique_ptr<State> GreedyParser::parse(const Sentence& sentence) {
  std::unique_ptr<State> state = std::make_unique<State>(sentence);
  while (!Transition::isTerminal(*state)) {
    // retrieve an one best action greedily
    Transition::apply(getNextAction(*state), state.get());
  }
  return state;
}

std::vector<std::unique_ptr<State>> GreedyParser::parse_batch(
    const std::vector<Sentence>& sentences) {
  std::vector<std::unique_ptr<State>> states;
  std::vector<State*> targets;
  states.reserve(sentences.size());
  targets.reserve(sentences.size());
  for (auto& sentence : sentences) {
    states.push_back(std::make_unique<State>(sentence));
    targets.push_back(states.back().get());
  }
  std::vector<State*> temp;
  std::vector<FeatureVector> features;
  temp.reserve(sentences.size());
  features.reserve(sentences.size());

  while (true) {
    temp.clear();
    temp.assign(targets.begin(), targets.end());
    targets.clear();
    features.clear();
    for (auto& state : temp) {
      if (!Transition::isTerminal(*state)) {
        targets.push_back(state);
        features.push_back(std::move(Feature::extract(*state)));
      }
    }
    if (targets.empty()) break;
    std::vector<std::vector<float>> score_matrix =
        classifier_->compute_batch(features);
    for (int i = 0; i < targets.size(); ++i) {
      int best_action = -1;
      float best_score = -INFINITY;
      for (unsigned action = 0; action < score_matrix[i].size(); ++action) {
        if (score_matrix[i][action] > best_score &&
            Transition::isAllowed(action, *targets[i])) {
          best_action = action;
          best_score = score_matrix[i][action];
        }
      }
      Transition::apply(best_action, targets[i]);
    }
  }
  return states;
}

Action GreedyParser::getNextAction(const State& state) {
  LOG_TRACE("{}", state);
  std::vector<float> scores =
      classifier_->compute(std::move(Feature::extract(state)));
  LOG_TRACE("scores: {}", scores);
  int best_action = -1;
  float best_score = -INFINITY;
  for (unsigned i = 0; i < scores.size(); ++i) {
    if (scores[i] > best_score && Transition::isAllowed(i, state)) {
      best_action = i;
      best_score = scores[i];
    }
  }
  LOG_TRACE("best action: {}", best_action);
  return static_cast<Action>(best_action);
}

}  // namespace coordparser
