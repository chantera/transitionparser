//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/parser.h"

#include <dynet/expr.h>
#include <dynet/tensor.h>

#include <algorithm>
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
  return parse_batch(sentences, sentences.size());
}

std::vector<std::unique_ptr<State>> GreedyParser::parse_batch(
    const std::vector<Sentence>& sentences, const size_t batch_size) {
  size_t num_sentences = sentences.size();
  size_t num_batches = num_sentences / batch_size + 1;

  std::vector<std::unique_ptr<State>> states;
  states.reserve(num_sentences);

  std::vector<int> indices(num_sentences);
  std::iota(std::begin(indices), std::end(indices), 0);
  std::sort(indices.begin(), indices.end(),
            [&sentences](size_t idx1, size_t idx2) {
              return sentences[idx1].length < sentences[idx2].length;
            });

  std::vector<State*> targets;
  targets.reserve(batch_size);
  std::vector<State*> temp;
  temp.reserve(batch_size);
  std::vector<FeatureVector> features;
  features.reserve(batch_size);

  for (int batch_index = 0; batch_index < num_batches; ++batch_index) {
    LOG_TRACE("parse batch {} of {}", batch_index + 1, num_batches);
    size_t offset = batch_index * batch_size;
    const size_t current_batch_size =
        std::min(num_sentences - offset, batch_size);

    targets.clear();
    temp.clear();
    features.clear();

    for (size_t i = offset; i < offset + current_batch_size; ++i) {
      auto state = std::make_unique<State>(sentences.at(indices[i]));
      targets.push_back(state.get());
      states.push_back(std::move(state));
    }

    while (true) {
      temp.clear();
      temp.assign(targets.begin(), targets.end());
      targets.clear();
      features.clear();
      for (const auto& state : temp) {
        if (!Transition::isTerminal(*state)) {
          targets.push_back(state);
          features.push_back(Feature::extract(*state));
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
