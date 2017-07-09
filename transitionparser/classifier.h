//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef TRANSITIONPARSER_CLASSIFIER_H_
#define TRANSITIONPARSER_CLASSIFIER_H_

#include <dynet/training.h>
#include <dynet/dynet.h>

#include <vector>
#include <utility>

#include "transitionparser/feature.h"
#include "transitionparser/transition.h"
#include "transitionparser/utility.h"

namespace transitionparser {

class State;

class Classifier {
 public:
  Classifier() {}
  virtual ~Classifier() {}

  virtual std::vector<float> compute(const FeatureVector& feature) = 0;

  virtual std::vector<std::vector<float>> compute_batch(
      const std::vector<FeatureVector>& features) = 0;

 private:
  DISALLOW_COPY_AND_MOVE(Classifier);
};

class NeuralClassifier : public Classifier {
 public:
  virtual void prepare(dynet::ComputationGraph* cg);

  std::vector<float> compute(const FeatureVector& feature) override;

  std::vector<std::vector<float>> compute_batch(
      const std::vector<FeatureVector>& features) override;

  virtual dynet::expr::Expression run(const std::vector<FeatureVector>& X) = 0;

 protected:
  dynet::ComputationGraph* cg_ = nullptr;
};

class MlpClassifier : public NeuralClassifier {
 public:
  MlpClassifier(dynet::Model& model,  // NOLINT(runtime/references)
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

  dynet::expr::Expression run(const std::vector<FeatureVector>& X) override;

 protected:
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

}  // namespace transitionparser

#endif  //  TRANSITIONPARSER_CLASSIFIER_H_
