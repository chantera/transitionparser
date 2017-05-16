//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/state.h"

namespace coordparser {

State::State(const Sentence& sentence) :
    sentence_(&sentence),
    num_tokens_((const unsigned int) sentence.tokens.size()),
    stack_{0},
    buffer_(1),
    heads_(num_tokens_, 0),
    labels_(num_tokens_, 0) {
  stack_.reserve(num_tokens_);
}

State::State(const State& prev_state,
             const Action& action,
             const std::vector<int>& stack,
             const int buffer,
             const std::vector<int>& heads,
             const std::vector<int>& labels) :
    sentence_(prev_state.sentence_),
    num_tokens_(prev_state.num_tokens_),
    stack_(stack),
    buffer_(buffer),
    heads_(heads),
    labels_(labels),
    history_(prev_state.history_.begin(), prev_state.history_.end()) {
  COORDPARSER_ASSERT(buffer_ <= num_tokens_, "buffer exceeds num_tokens.");
  history_.push_back(action);
}

std::ostream& operator<<(std::ostream& os, const State& state) {
  const Token& pad = Token::createPad();
  const Token& s0 = state.getToken(state.stack(0), pad);
  const Token& s1 = state.getToken(state.stack(1), pad);
  const Token& b0 = state.getToken(state.buffer(0), pad);
  const Token& b1 = state.getToken(state.buffer(1), pad);
  os << utility::string::format("step={}, s0: {}, s1: {}, b0: {}, b1: {}",
                                state.step(), s0, s1, b0, b1);
  return os;
}

void State::advance() {
  ++buffer_;
}

void State::push(int index) {
  stack_.push_back(index);
}

int State::pop() {
  const int index = stack_.back();
  stack_.pop_back();
  return index;
}

void State::addArc(int index, int head, int label) {
  heads_[index] = head;
  labels_[index] = label;
}

void State::record(Action action) {
  history_.push_back(action);
}

int State::step() const {
  return history_.size();
}

int State::numTokens() const {
  return num_tokens_;
}

bool State::end() const {
  return buffer_ == num_tokens_;
}

int State::top() const {
  return stack_.back();
}

int State::stack(int position) const {
  if (position < 0) return -1;
  const int index = stackSize() - 1 - position;
  return index < 0 ? -1 : stack_[index];
}

int State::stackSize() const {
  return stack_.size();
}

bool State::stackEmpty() const {
  return stack_.empty();
}

int State::buffer() const {
  return buffer_;
}

int State::buffer(int position) const {
  if (position < 0) return -1;
  const int index = buffer_ + position;
  return index < num_tokens_ ? -1 : index;
}

int State::head(int index) const {
  return heads_[index];
}

int State::label(int index) const {
  return labels_[index];
}

int State::leftmost(int index, int from) const {
  if (index >=0 && index < num_tokens_ && from >= 0 && from < index) {
    for (int i = from; i < index; i++) {
      if (heads_[i] == index) {
        return i;
      }
    }
  }
  return -1;
}

int State::rightmost(int index, int from) const {
  from = from == -1 ? num_tokens_ - 1 : from;
  if (index >= 0 && index < num_tokens_
      && from > index && from < num_tokens_) {
    for (int i = from; i > index; i--) {
      if (heads_[i] == index) {
        return i;
      }
    }
  }
  return -1;
}

const Token& State::getToken(int index) const {
  return sentence_->tokens.at(index);
}

const Token& State::getToken(int index, const Token& default_token) const {
  if (index < 0 || index >= num_tokens_) return default_token;
  return sentence_->tokens[index];
}

const std::vector<Action>& State::history() const {
  return history_;
}

}  // namespace coordparser
