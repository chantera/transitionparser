//
// Created by Hiroki Teranishi on 4/6/17.
//

#include "state.h"

namespace coordparser {

State::State(const Sentence& sentence) :
    step_(0),
    tokens_(&sentence.tokens),
    token_length_((const int) tokens_->size()),
    stack_{0},
    buffer_head_(1) {}

State::State(const std::shared_ptr<State>& prev_state,
             const Action& prev_action,
             const Arc& prev_arc,
             const std::deque<int>& stack,
             const int buffer_head) :
    step_(prev_state->step_ + 1),
    tokens_(prev_state->tokens_),
    token_length_((const int) tokens_->size()),
    stack_(stack),
    buffer_head_(buffer_head) {}

bool State::isTerminal() {
  return true;
}

const Token& State::getStackToken(const int position) const {
  return (Token &) tokens_->operator[](stack_.at(position));
}

const Token& State::getBufferToken(const int position) const {
  return (Token &) tokens_->operator[](buffer_head_ + position);
}

const Token& State::getLeftmostToken(const int index) const {
  return (Token &) tokens_->operator[](leftmost_[index]);
}

const Token& State::getRightmostToken(const int index) const {
  return (Token &) tokens_->operator[](rightmost_[index]);
}

const Feature* State::getFeature() const {
  if (!feature_) {
    feature_.reset(new Feature(*this));
  }
  return feature_.get();
}

}
