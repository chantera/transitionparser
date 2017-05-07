//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/sentence.h"

#include <vector>

namespace coordparser {

Sentence::Sentence(const int id, const std::vector<Token>& tokens) :
    id(id), tokens(tokens) {}

std::ostream& operator<<(std::ostream& os, const Sentence& sentence) {
  os << utility::vector::join(sentence.tokens, ' ');
  return os;
}

}  // namespace coordparser
