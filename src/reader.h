//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_READER_H
#define COORDPARSER_READER_H

#include <string>
#include <vector>
#include "sentence.h"

namespace coordparser {

class Reader {

public:
    Reader(const std::string& filepath);

    virtual std::vector<Sentence> read() = 0;

protected:
    const std::string filepath;
};

}

#endif //COORDPARSER_READER_H
