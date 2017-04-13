//
// Created by Hiroki Teranishi on 4/6/17.
//

#ifndef COORDPARSER_STATE_H
#define COORDPARSER_STATE_H

#include "sentence.h"

namespace coordparser {

class State {

public:
  const std::shared_ptr<State> prev_state_;

  explicit State(const Sentence& sentence);

  bool isTerminal();

private:
  // const std::vector<Token>& tokens;

};

}

#endif //COORDPARSER_STATE_H
