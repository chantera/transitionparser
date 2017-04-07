//
// Created by Hiroki Teranishi on 4/6/17.
//

#ifndef COORDPARSER_PARSER_H
#define COORDPARSER_PARSER_H

#include "classifier.h"
#include "sentence.h"
#include "state.h"

namespace coordparser {

class Parser {

public:
    virtual State parse(Sentence sentence) = 0;

protected:
    Classifier* classifier;
};

}

#endif //COORDPARSER_PARSER_H
