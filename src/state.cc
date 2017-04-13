//
// Created by Hiroki Teranishi on 4/6/17.
//

#include "state.h"

namespace coordparser {

State::State(const Sentence& sentence) /*: tokens(sentence.tokens)*/ {

}

bool State::isTerminal() {
  return true;
}

}
