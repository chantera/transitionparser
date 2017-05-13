//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/reader.h"

#include <fstream>
#include <iostream>
#include <utility>

namespace coordparser {

Reader::Reader(const std::string& filepath) : filepath(filepath) {}

ConllReader::ConllReader(const std::string& filepath) : Reader(filepath) {}

std::vector<Sentence> ConllReader::read() {
  std::ifstream ifs(filepath);
  if (ifs.fail()) {
    // @TODO
  }

  std::vector<Sentence> sentences;
  std::vector<Token> tokens;
  tokens.push_back(std::move(Token::createRoot()));
  std::string line;
  int count = 0;

  while (getline(ifs, line)) {
    utility::string::trim(line);
    if (line.length() == 0) {
      if (tokens.size() > 1) {
        sentences.emplace_back(++count, tokens);
        tokens.clear();
        tokens.push_back(std::move(Token::createRoot()));
      }
    } else {
      tokens.emplace_back(utility::string::split(line, kDelimiter));
    }
  }
  if (tokens.size() > 1) {
    sentences.emplace_back(++count, tokens);
  }
  return sentences;
}

}  // namespace coordparser
