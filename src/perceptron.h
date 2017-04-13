//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_PERCEPTRON_H
#define COORDPARSER_PERCEPTRON_H

#include "classifier.h"

namespace coordparser {

class Perceptron : public Classifier {

public:
    Action getNextAction(const State& state) override ;
};

}

#endif //COORDPARSER_PERCEPTRON_H
