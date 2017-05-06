//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/state.h"

namespace coordparser {

State::State(const Sentence* sentence) :
    step_(0),
    sentence_(sentence),
    num_tokens_((const unsigned int) sentence->tokens.size()),
    stack_{0},
    buffer_(1),
    heads_(num_tokens_, -1),
    labels_(num_tokens_, -1),
    prev_state_(),
    action_(NoneAction) {}

State::State(const std::shared_ptr<State>& prev_state,
             const Action& action,
             const std::vector<int>& stack,
             const int buffer,
             const std::vector<int>& heads,
             const std::vector<int>& labels) :
    step_(prev_state->step_ + 1),
    sentence_(prev_state->sentence_),
    num_tokens_(prev_state->num_tokens_),
    stack_(stack),
    buffer_(buffer),
    heads_(heads),
    labels_(labels),
    prev_state_(prev_state),
    action_(action) {}

bool State::isTerminal() {
  return buffer_ == num_tokens_;
}

const Token& State::getStackToken(const unsigned position,
                                  const Token& default_token) const {
  const unsigned bound = static_cast<const unsigned>(stack_.size() - 1);
  if (position > bound) {
    return default_token;
  }
  return (Token &) sentence_->tokens[stack_[bound - position]];
}

const Token& State::getBufferToken(const unsigned position,
                                   const Token& default_token) const {
  const unsigned index = buffer_ + position;
  if (index >= num_tokens_) {
    return default_token;
  }
  return (Token &) sentence_->tokens[index];
}

const Token& State::getLeftmostToken(const unsigned index,
                                     const Token& default_token) const {
  if (index < num_tokens_) {
    for (int i = 0; i < index; i++) {
      if (heads_[i] == index) {
        return (Token&) sentence_->tokens[i];
      }
    }
  }
  return default_token;
}

const Token& State::getRightmostToken(const unsigned index,
                                      const Token& default_token) const {
  if (index < num_tokens_) {
    for (int i = num_tokens_ - 1; i > index; i--) {
      if (heads_[i] == index) {
        return (Token&) sentence_->tokens[i];
      }
    }
  }
  return default_token;
}

const Feature* State::getFeature() const {
  if (!feature_) {
    feature_.reset(new Feature(*this));
  }
  return feature_.get();
}

}  // namespace coordparser
