//
// Created by Hiroki Teranishi on 4/6/17.
//

#include <iostream>
#include "greedy_parser.h"
#include "logger.h"

namespace coordparser {

State GreedyParser::parse(Sentence sentence) {
    std::cout << sentence << std::endl;
    Logger::info("%s", sentence);
    State state;
    while (!state.isTerminal()) {
        Action action = classifier->getNextAction(state); // retrieve an one best action greedily
        state = action.apply(state);
    }
    return state;
}

}
