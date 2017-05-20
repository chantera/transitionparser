//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_STATE_H_
#define COORDPARSER_STATE_H_

#include <deque>
#include <memory>
#include <set>
#include <stack>
#include <vector>

#include "coordparser/transition.h"
#include "coordparser/sentence.h"

namespace coordparser {

typedef int Action;
class Feature;

class State {
 public:
  State() = delete;

  explicit State(const Sentence& sentence);

  State(const State& prev_state,
        const Action& action,
        const std::vector<int>& stack,
        const int buffer,
        const std::vector<int>& heads,
        const std::vector<int>& labels);

  DEFAULT_MOVE_AND_ASSIGN(State);

  ~State() = default;

  friend std::ostream& operator<<(std::ostream& os, const State& state);

  template<typename OStream>
  friend OStream& operator<<(OStream& os, const State& state) {
    std::stringstream ss;
    ss << state;
    os << ss.str();
    return os;
  }

  void advance();

  void push(int index);

  int pop();

  void addArc(int index, int head, int label);

  void record(Action action);

  int step() const;

  int numTokens() const;

  bool end() const;

  int top() const;

  int stack(int position) const;

  int stackSize() const;

  bool stackEmpty() const;

  int buffer() const;

  int buffer(int position) const;

  int head(int index) const;

  const std::vector<int>& heads() const;

  int label(int index) const;

  const std::vector<int>& labels() const;

  int leftmost(int index, int from = 0) const;

  int rightmost(int index, int from = -1) const;

  const Token& getToken(int index) const;

  const Token& getToken(int index, const Token& default_token) const;

  const std::vector<Action>& history() const;

 private:
  const Sentence* sentence_;
  const int num_tokens_;
  std::vector<int> stack_;
  int buffer_;
  std::vector<int> heads_;
  std::vector<int> labels_;
  double score_ = 0.0;
  std::vector<Action> history_;

  DISALLOW_COPY_AND_ASSIGN(State);
};

}  // namespace coordparser

#endif  // COORDPARSER_STATE_H_
