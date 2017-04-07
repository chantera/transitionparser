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
    Sentence(const int id, std::vector<Token>& tokens);

protected:
    const int id;
    const std::vector<Token> tokens;
};

}

#endif //COORDPARSER_SENTENCE_H
