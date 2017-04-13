//
// Created by Hiroki Teranishi on 4/6/17.
//

#include "logger.h"
#include "greedy_parser.h"

namespace coordparser {

State GreedyParser::parse(const Sentence& sentence) {
  State state(sentence);
  while (!state.isTerminal()) {
    Action action = classifier_->getNextAction(state); // retrieve an one best action greedily
    //state = action.apply(state);
  }
  return state;
}

}
