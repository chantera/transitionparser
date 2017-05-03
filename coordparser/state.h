//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_STATE_H_
#define COORDPARSER_STATE_H_

#include <deque>
#include <memory>
#include <stack>
#include <vector>

#include "coordparser/arc.h"
#include "coordparser/action.h"
#include "coordparser/feature.h"
#include "coordparser/sentence.h"

namespace coordparser {

class Action;
class Feature;

class State {
 public:
  const int step_;
  const std::vector<Token>* tokens_;
  const int token_length_;
  const std::deque<int> stack_;
  const int buffer_head_;
  const std::vector<int> leftmost_;
  const std::vector<int> rightmost_;
  // const std::vector<int> features_;
  // const std::set<Action> possible_actions_;
  // const std::shared_ptr<State> prev_state_;
  // const Action prev_action_;
  // const double score_;

  explicit State(const Sentence& sentence);

  State(const std::shared_ptr<State>& prev_state,
        const Action& prev_action,
        const Arc& prev_arc,
        const std::deque<int>& stack,
        const int buffer_head);

  bool isTerminal();

  const Token& getStackToken(const int position) const;

  const Token& getBufferToken(const int position) const;

  const Token& getLeftmostToken(const int index) const;

  const Token& getRightmostToken(const int index) const;

  const Feature* getFeature() const;

 private:
  mutable std::unique_ptr<Feature> feature_;
};

}  // namespace coordparser

#endif  // COORDPARSER_STATE_H_
