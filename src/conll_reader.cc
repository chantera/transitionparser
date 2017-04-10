//
// Created by Hiroki Teranishi on 4/7/17.
//

#include <fstream>
#include "conll_reader.h"
#include "utility.h"

namespace coordparser {

ConllReader::ConllReader(const std::string& filepath) : Reader(filepath) {}

std::vector<Sentence> ConllReader::read() {
    std::ifstream ifs(filepath);
    if (ifs.fail()) {
        // @TODO
    }

    std::vector<Sentence> sentences;
    std::vector<Token> tokens;
    std::string line;
    int count = 0;

    while (getline(ifs, line)) {
        utility::string::trim(line);
        if (line.length() == 0) {
            if (tokens.size() > 1) {
                sentences.push_back(Sentence(++count, tokens));
                tokens.clear();
                tokens.push_back(Token::createRoot());
            }
        } else {
            tokens.push_back(Token(utility::string::split(line, DELIMITER)));
        }
    }
    if (tokens.size() > 1) {
        sentences.push_back(Sentence(++count, tokens));
    }
    return sentences;
}

}
