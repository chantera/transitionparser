//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_PARSER_H_
#define COORDPARSER_PARSER_H_

#include <memory>
#include <vector>

#include "coordparser/classifier.h"
#include "coordparser/sentence.h"
#include "coordparser/state.h"
#include "coordparser/utility.h"

namespace coordparser {

class Parser {
 public:
  Parser() = delete;
  virtual ~Parser() {}

  explicit Parser(std::shared_ptr<Classifier> classifier);

  virtual std::unique_ptr<State> parse(const Sentence& sentence) = 0;

 protected:
  const std::shared_ptr<Classifier> classifier_;

 private:
  DISALLOW_COPY_AND_MOVE(Parser);
};

class GreedyParser : public Parser {
 public:
  explicit GreedyParser(std::shared_ptr<Classifier> classifier);

  std::unique_ptr<State> parse(const Sentence& sentence) override;

  std::vector<std::unique_ptr<State>> parse_batch(
      const std::vector<Sentence>& sentences);

  std::vector<std::unique_ptr<State>> parse_batch(
      const std::vector<Sentence>& sentences, const size_t batch_size);

  Action getNextAction(const State& state);
};

}  // namespace coordparser

#endif  // COORDPARSER_PARSER_H_
