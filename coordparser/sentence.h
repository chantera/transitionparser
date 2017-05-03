//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_SENTENCE_H
#define COORDPARSER_SENTENCE_H

#include <vector>
#include "token.h"
#include "utility.h"

namespace coordparser {

class Sentence {

public:
  Sentence() = delete;
  DEFAULT_COPY_AND_MOVE(Sentence);
  ~Sentence() {};

  const int id;
  std::vector<Token> tokens;

  Sentence(const int id, const std::vector<Token>& tokens);

  friend std::ostream& operator<<(std::ostream& os, const Sentence& sentence);
};

}

#endif //COORDPARSER_SENTENCE_H
