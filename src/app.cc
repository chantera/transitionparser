//
// Created by Hiroki Teranishi on 3/2/17.
//

#include <iostream>
#include "app.h"
#include "logger.h"
#include "greedy_parser.h"
#include "conll_reader.h"

namespace coordparser {

App::App() {}

void App::train() {
  Logger::log(Logger::LogLevel::INFO, "Hello, World!");

  std::string filepath = "/Users/hiroki/Desktop/NLP/data/TreeBank3_stanford/dep/wsj_02.conll";
  ConllReader reader(filepath);

  std::unique_ptr<Parser> parser(new GreedyParser());

  const std::vector<Sentence> sentences = reader.read();
  for (auto& sentence : sentences) {
      parser->parse(sentence);
  }
}

}