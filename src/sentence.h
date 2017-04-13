//
// Created by Hiroki Teranishi on 4/6/17.
//

#ifndef COORDPARSER_SENTENCE_H
#define COORDPARSER_SENTENCE_H

#include <vector>
#include "token.h"

namespace coordparser {

class Sentence {

public:
  const int id;
  std::vector<Token> tokens;

  Sentence(const int id, const std::vector<Token>& tokens);

  friend std::ostream& operator<<(std::ostream& os, const Sentence& sentence);
};

}

#endif //COORDPARSER_SENTENCE_H
