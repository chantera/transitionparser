//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_ACTION_H
#define COORDPARSER_ACTION_H

#include "state.h"

namespace coordparser {

class Action {

public:
    enum {
        REDUCE = 0,
        SHIFT  = 1,
        RIGHT  = 2,
        LEFT   = 3,
    };

    std::shared_ptr<State> apply(const std::shared_ptr<State>& state);
};

}

#endif //COORDPARSER_ACTION_H
