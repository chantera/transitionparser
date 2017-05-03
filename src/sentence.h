//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_SENTENCE_H
#define COORDPARSER_SENTENCE_H

#include <vector>
#include "token.h"

namespace coordparser {

class Sentence {

public:
  Sentence() = delete;
  ~Sentence() {};

  const int id;
  std::vector<Token> tokens;

  Sentence(const int id, const std::vector<Token>& tokens);

  friend std::ostream& operator<<(std::ostream& os, const Sentence& sentence);
};

}

#endif //COORDPARSER_SENTENCE_H
