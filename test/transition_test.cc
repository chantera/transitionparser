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
    State state(sentence);
    while (!Transition::isTerminal(state)) {
      Action action = Transition::getOracle(state);
      Transition::apply(action, &state);
    }
    int correct = 0;
    for (int i = 0; i < state.numTokens(); ++i) {
      if (state.head(i) == state.getToken(i).head &&
          state.label(i) == state.getToken(i).label) {
        ++correct;
      }
    }
    ASSERT_TRUE(correct == state.numTokens());
  }
}
