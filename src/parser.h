//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_PARSER_H
#define COORDPARSER_PARSER_H

#include "classifier.h"
#include "state.h"
#include "utility.h"

namespace coordparser {

class Sentence;

class Parser {
  DISALLOW_COPY_AND_MOVE(Parser);

public:
  Parser() {};
  virtual ~Parser() {};

  virtual State parse(const Sentence& sentence) = 0;

protected:
  const std::unique_ptr<Classifier> classifier_;
};

class GreedyParser : public Parser {

public:
  State parse(const Sentence& sentence) override;
};

}

#endif //COORDPARSER_PARSER_H
