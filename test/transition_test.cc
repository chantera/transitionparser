//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include <coordparser/reader.h>
#include <coordparser/sentence.h>
#include <coordparser/state.h>
#include <coordparser/transition.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace coordparser;  // NOLINT(build/namespaces)

class TransitionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    std::string filepath =
        "/Users/hiroki/Desktop/NLP/data/archive.20161120/"
            "penn_treebank/dep/stanford/section/parse-train.conll";
    ConllReader reader(filepath);
    sentences_ = reader.read();
  }

  virtual void TearDown() {}

  std::vector<Sentence> sentences_;
};

TEST_F(TransitionTest, Oracle) {
  for (auto& sentence : sentences_) {
    std::cout << sentence << std::endl;
    std::vector<int> gold_heads;
    std::vector<int> gold_labels;
    for (auto& token : sentence.tokens) {
      gold_heads.push_back(token.head);
      gold_labels.push_back(token.deprel);
    }
    std::shared_ptr<coordparser::State> state
        = std::make_shared<coordparser::State>(&sentence);
    while (!state->isTerminal()) {
      Action action = Transition::getOracle(*state);
      std::cout << state->stack_ << "] [" << state->buffer_
                << "..., action: " << action << std::endl;
      Transition::apply(action, state);
    }
    // std::cout << gold_heads << std::endl;
    // std::cout << state->heads_ << std::endl;
    ASSERT_THAT(state->heads_, ::testing::ContainerEq(gold_heads));
    ASSERT_THAT(state->labels_, ::testing::ContainerEq(gold_labels));
  }
}
