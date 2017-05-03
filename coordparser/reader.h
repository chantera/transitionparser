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
  Reader() = delete;
  explicit Reader(const std::string& filepath);
  virtual ~Reader() {}

  virtual std::vector<Sentence> read() = 0;

 protected:
  const std::string filepath;

 private:
  DISALLOW_COPY_AND_MOVE(Reader);
};

class ConllReader : public Reader {
 public:
  ConllReader() = delete;
  explicit ConllReader(const std::string& filepath);

  std::vector<Sentence> read() override;

 private:
  static const char kDelimiter = '\t';
};

}  // namespace coordparser

#endif  // COORDPARSER_READER_H_
