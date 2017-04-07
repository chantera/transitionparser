//
// Created by Hiroki Teranishi on 4/6/17.
//

#ifndef COORDPARSER_CLASSIFIER_H
#define COORDPARSER_CLASSIFIER_H

#include "state.h"
#include "action.h"

namespace coordparser {

class Classifier {

public:
    virtual Action getNextAction(State state) = 0;
};

}

#endif //COORDPARSER_CLASSIFIER_H
