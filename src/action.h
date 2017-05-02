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
        SHIFT  = 0,
        RIGHT  = 1,
        LEFT   = 2,
    };

    std::shared_ptr<State> apply(const std::shared_ptr<State>& state);
};

}

#endif //COORDPARSER_ACTION_H
