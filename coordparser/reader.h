//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_READER_H_
#define COORDPARSER_READER_H_

#include <string>
#include <vector>

#include "coordparser/sentence.h"
#include "coordparser/utility.h"

namespace coordparser {

class Reader {
 public:
  Reader() {}
  explicit Reader(const std::string& filepath);
  virtual ~Reader() {}

  virtual std::vector<Sentence> read() = 0;

 protected:
  std::string filepath_;

 private:
  DISALLOW_COPY_AND_MOVE(Reader);
};

class ConllReader : public Reader {
 public:
  ConllReader() {}
  explicit ConllReader(const std::string& filepath);

  std::vector<Sentence> read() override;

  std::vector<Sentence> read(const std::string& filepath);

 private:
  static const char kDelimiter = '\t';
};

}  // namespace coordparser

#endif  // COORDPARSER_READER_H_
