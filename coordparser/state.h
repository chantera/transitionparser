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
#include "coordparser/feature.h"
#include "coordparser/sentence.h"

namespace coordparser {

typedef int Action;
class Feature;

class State {
 public:
  const int step_;
  const Sentence* sentence_;
  const unsigned num_tokens_;
  const std::vector<int> stack_;
  const int buffer_;
  const std::vector<int> heads_;
  const std::vector<int> labels_;

  State() = delete;

  explicit State(const Sentence* sentence);

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

  bool isTerminal() const;

  const Token& getToken(unsigned index) const;

  const Token& getStackToken(unsigned position) const;

  const Token& getStackToken(unsigned position,
                             const Token& default_token) const;

  const Token& getBufferToken(unsigned position) const;

  const Token& getBufferToken(unsigned position,
                              const Token& default_token) const;

  const Token& getLeftmostToken(int index,
                                const Token& default_token,
                                int from = 0) const;

  const Token& getRightmostToken(int index,
                                 const Token& default_token,
                                 int from = -1) const;

  const Feature* getFeature() const;

  const std::vector<Action>& history() const;

 private:
  mutable std::unique_ptr<Feature> feature_;
  double score_ = 0.0;
  std::vector<Action> history_;

  DISALLOW_COPY_AND_ASSIGN(State);
};

}  // namespace coordparser

#endif  // COORDPARSER_STATE_H_
