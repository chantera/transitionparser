//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_READER_H
#define COORDPARSER_READER_H

#include <string>
#include <vector>
#include "sentence.h"
#include "utility.h"

namespace coordparser {

class Reader {
  DISALLOW_COPY_AND_MOVE(Reader);

public:
  Reader() = delete;
  virtual ~Reader() {};

  explicit Reader(const std::string& filepath);

  virtual std::vector<Sentence> read() = 0;

protected:
  const std::string filepath;
};

class ConllReader : public Reader {

public:
  ConllReader() = delete;

  explicit ConllReader(const std::string& filepath) ;

  std::vector<Sentence> read() override;

private:
  static const char kDelimiter = '\t';
};

}

#endif //COORDPARSER_READER_H
