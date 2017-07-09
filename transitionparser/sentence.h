//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef TRANSITIONPARSER_SENTENCE_H_
#define TRANSITIONPARSER_SENTENCE_H_

#include <vector>

#include "transitionparser/token.h"
#include "transitionparser/utility.h"

namespace transitionparser {

struct Sentence {
 public:
  Sentence() = delete;
  Sentence(const int id, const std::vector<Token>& tokens);
  DEFAULT_COPY_AND_MOVE(Sentence);
  ~Sentence() {}

  friend std::ostream& operator<<(std::ostream& os, const Sentence& sentence);

  const int id;
  const std::vector<Token> tokens;
  const unsigned length;
};

}  // namespace transitionparser

#endif  // TRANSITIONPARSER_SENTENCE_H_
