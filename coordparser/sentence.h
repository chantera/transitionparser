//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_SENTENCE_H_
#define COORDPARSER_SENTENCE_H_

#include <vector>

#include "coordparser/token.h"
#include "coordparser/utility.h"

namespace coordparser {

struct Sentence {
 public:
  Sentence() = delete;
  Sentence(const int id, const std::vector<Token>& tokens);
  DEFAULT_COPY_AND_MOVE(Sentence);
  ~Sentence() {}

  friend std::ostream& operator<<(std::ostream& os, const Sentence& sentence);

  const int id;
  const std::vector<Token> tokens;
};

}  // namespace coordparser

#endif  // COORDPARSER_SENTENCE_H_
