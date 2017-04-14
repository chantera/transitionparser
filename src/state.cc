//
// Created by Hiroki Teranishi on 4/6/17.
//

#include "state.h"

namespace coordparser {

State::State(const Sentence& sentence)
    : tokens_(&sentence.tokens)  {

}

State::State(const std::shared_ptr<State>& prev_state,
             const Action &prev_action,
             const Arc &prev_arc,
             const std::stack<int> &stack,
             const int buffer_head) : tokens_(prev_state->tokens_) {

}

bool State::isTerminal() {
  return true;
}

}
