//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_CONLL_READER_H
#define COORDPARSER_CONLL_READER_H

#include "reader.h"

namespace coordparser {

class ConllReader : public Reader {

public:
    ConllReader(const std::string& filepath);

    std::vector<Sentence> read() override;

private:
    static const char DELIMITER = '\t';
};

}

#endif //COORDPARSER_CONLL_READER_H
