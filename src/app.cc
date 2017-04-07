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
    Logger::log(Logger::LogLevel::INFO, "int: %d", 10);
    Logger::info("float: %f", 2.3);

    std::string filepath = "/Users/hiroki/Desktop/NLP/data/TreeBank3_stanford/dep/wsj_02.conll";
    ConllReader reader(filepath);
    std::vector<Sentence> sentences = reader.read();
    std::cout << &filepath << std::endl;

    // Parser* parser = new GreedyParser();
    // Sentence sentence;
    // parser->parse(sentence);
}

}