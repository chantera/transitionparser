//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/classifier.h"

#include <dynet/expr.h>

#include "coordparser/logger.h"

namespace DE = dynet::expr;

namespace coordparser {

void NeuralClassifier::prepare(dynet::ComputationGraph* cg) {
  cg_ = cg;
}

std::vector<float> NeuralClassifier::compute(const FeatureVector& feature) {
  LOG_TRACE("feature: {}", feature);
  cg_->clear();
  return dynet::as_vector(cg_->forward(run({feature})));
}

std::vector<std::vector<float>> NeuralClassifier::compute_batch(
    const std::vector<FeatureVector>& features) {
  int batch_size = features.size();
  std::vector<std::vector<float>> score_matrix;
  score_matrix.reserve(batch_size);

  cg_->clear();
  auto v = dynet::as_vector(cg_->forward(run(features)));
  int dim = v.size() / batch_size;
  auto start = v.begin();
  for (int i = 0; i < batch_size; ++i) {
    score_matrix.emplace_back(std::make_move_iterator(start),
                              std::make_move_iterator(start += dim));
  }
  return score_matrix;
}

MlpClassifier::MlpClassifier(dynet::Model& model,
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
                             const unsigned output_size) :
    word_vocab_size_(word_vocab_size),
    word_embed_size_(word_embed_size),
    word_feature_size_(word_feature_size),
    pos_vocab_size_(pos_vocab_size),
    pos_embed_size_(pos_embed_size),
    pos_feature_size_(pos_feature_size),
    label_vocab_size_(label_vocab_size),
    label_embed_size_(label_embed_size),
    label_feature_size_(label_feature_size),
    hidden1_size_(hidden1_size),
    hidden2_size_(hidden2_size),
    output_size_(output_size),
    p_lookup_w_(model.add_lookup_parameters(word_vocab_size,
                                            {word_embed_size})),
    p_lookup_p_(model.add_lookup_parameters(pos_vocab_size, {pos_embed_size})),
    p_lookup_l_(model.add_lookup_parameters(label_vocab_size,
                                            {label_embed_size})),
    p_W1_(model.add_parameters({hidden1_size,
                                word_feature_size * word_embed_size
                                    + pos_feature_size * pos_embed_size
                                    + label_feature_size * label_embed_size})),
    p_b1_(model.add_parameters({hidden1_size})),
    p_W2_(model.add_parameters({hidden2_size, hidden1_size})),
    p_b2_(model.add_parameters({hidden2_size})),
    p_W3_(model.add_parameters({output_size, hidden2_size})),
    p_b3_(model.add_parameters({output_size})) {}

DE::Expression MlpClassifier::run(const std::vector<FeatureVector>& X) {
  std::vector<DE::Expression> embeddings;
  auto feature_tensor = Feature::unpackFeatures(X);
  for (auto feature_batch : feature_tensor[0]) {
    embeddings.push_back(DE::lookup(*cg_, p_lookup_w_, feature_batch));
  }
  for (auto feature_batch : feature_tensor[1]) {
    embeddings.push_back(DE::lookup(*cg_, p_lookup_p_, feature_batch));
  }
  for (auto feature_batch : feature_tensor[2]) {
    embeddings.push_back(DE::lookup(*cg_, p_lookup_l_, feature_batch));
  }
  DE::Expression h0 = DE::concatenate(embeddings);

  DE::Expression W1 = DE::parameter(*cg_, p_W1_);
  DE::Expression b1 = DE::parameter(*cg_, p_b1_);
  DE::Expression h1 = DE::rectify(W1 * h0 + b1);

  DE::Expression W2 = DE::parameter(*cg_, p_W2_);
  DE::Expression b2 = DE::parameter(*cg_, p_b2_);
  DE::Expression h2 = DE::rectify(W2 * h1 + b2);

  DE::Expression W3 = DE::parameter(*cg_, p_W3_);
  DE::Expression b3 = DE::parameter(*cg_, p_b3_);
  DE::Expression y = W3 * h2 + b3;

  return y;
}

}  // namespace coordparser
