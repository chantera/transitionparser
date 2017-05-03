//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_CLASSIFIER_H
#define COORDPARSER_CLASSIFIER_H

#include <dynet/training.h>
#include <dynet/dynet.h>
#include "action.h"
#include "utility.h"

namespace coordparser {

class State;

class Classifier {
  DISALLOW_COPY_AND_MOVE(Classifier);

public:
  Classifier() {}
  virtual ~Classifier() {}

  virtual Action getNextAction(const State& state) = 0;
};

class NeuralClassifier : public Classifier {

public:
  virtual void prepare(dynet::ComputationGraph* cg);

protected:
  dynet::ComputationGraph* cg_ = nullptr;

};

class MlpClassifier : public NeuralClassifier {

public:
  MlpClassifier(dynet::Model& model,
                const unsigned word_vocab_size,
                const unsigned word_embed_size,
                const unsigned word_feature_size,
                const unsigned pos_vocab_size,
                const unsigned pos_embed_size,
                const unsigned pos_feature_size,
                const unsigned label_vocab_size,
                const unsigned label_embed_size,
                const unsigned label_feature_size,
                const unsigned hidden1_size,
                const unsigned hidden2_size,
                const unsigned n_labels);

  void prepare(dynet::ComputationGraph* cg) override;

  Action getNextAction(const State& state) override;

private:
  class MLP {

  public:
    const unsigned word_vocab_size_;
    const unsigned word_embed_size_;
    const unsigned word_feature_size_;
    const unsigned pos_vocab_size_;
    const unsigned pos_embed_size_;
    const unsigned pos_feature_size_;
    const unsigned label_vocab_size_;
    const unsigned label_embed_size_;
    const unsigned label_feature_size_;
    const unsigned hidden1_size_;
    const unsigned hidden2_size_;
    const unsigned output_size_;

    MLP(dynet::Model& model,
        const unsigned word_vocab_size,
        const unsigned word_embed_size,
        const unsigned word_feature_size,
        const unsigned pos_vocab_size,
        const unsigned pos_embed_size,
        const unsigned pos_feature_size,
        const unsigned label_vocab_size,
        const unsigned label_embed_size,
        const unsigned label_feature_size,
        const unsigned hidden1_size,
        const unsigned hidden2_size,
        const unsigned output_size);

    dynet::expr::Expression forward(const std::vector<unsigned>& X_w,
                                    const std::vector<unsigned>& X_p,
                                    const std::vector<unsigned>& X_l,
                                    dynet::ComputationGraph& cg);

  private:
    dynet::LookupParameter p_lookup_w_;
    dynet::LookupParameter p_lookup_p_;
    dynet::LookupParameter p_lookup_l_;
    dynet::Parameter p_W1_;
    dynet::Parameter p_b1_;
    dynet::Parameter p_W2_;
    dynet::Parameter p_b2_;
    dynet::Parameter p_W3_;
    dynet::Parameter p_b3_;
  };

  MLP mlp_;
};

}

#endif //COORDPARSER_CLASSIFIER_H
