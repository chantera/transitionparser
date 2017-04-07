//
// Created by Hiroki Teranishi on 4/6/17.
//

#include <vector>
#include "sentence.h"

namespace coordparser {

Sentence::Sentence(const int id, std::vector<Token>& tokens) : id(id), tokens(tokens) {}

}
