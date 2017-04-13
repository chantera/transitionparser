//
// Created by Hiroki Teranishi on 4/6/17.
//

#ifndef COORDPARSER_GREEDY_PARSER_H
#define COORDPARSER_GREEDY_PARSER_H

#include "parser.h"

namespace coordparser {

class GreedyParser : public Parser {

public:
    State parse(const Sentence& sentence) override;
};

}

#endif //COORDPARSER_GREEDY_PARSER_H
