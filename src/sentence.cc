//
// Created by Hiroki Teranishi on 4/6/17.
//

#include <vector>
#include "sentence.h"
#include "utility.h"

namespace coordparser {

Sentence::Sentence(const int id, const std::vector<Token>& tokens) : id(id), tokens(tokens) {}

std::ostream& operator<<(std::ostream& os, const Sentence& sentence) {
    os << utility::vector::join(sentence.tokens, ' ');
    return os;
}

}
