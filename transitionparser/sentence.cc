//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "transitionparser/sentence.h"

#include <vector>

namespace transitionparser {

Sentence::Sentence(const int id, const std::vector<Token>& tokens) :
    id(id), tokens(tokens),
    length(static_cast<const unsigned int>(tokens.size())) {}

std::ostream& operator<<(std::ostream& os, const Sentence& sentence) {
  os << utility::vector::join(sentence.tokens, ' ');
  return os;
}

}  // namespace transitionparser
