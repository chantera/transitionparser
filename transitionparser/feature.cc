//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "transitionparser/feature.h"

#include <utility>

namespace transitionparser {

const unsigned Feature::kNWordFeatures  = 20;
const unsigned Feature::kNPosFeatures   = 20;
const unsigned Feature::kNLabelFeatures = 12;

FeatureVector Feature::extract(const State &state) {
  Token pad = Token::createPad();
  const Token& s0 = state.getToken(state.stack(0), pad);
  const Token& s1 = state.getToken(state.stack(1), pad);
  const Token& s2 = state.getToken(state.stack(2), pad);
  const Token& s3 = state.getToken(state.stack(3), pad);
  const Token& b0 = state.getToken(state.buffer(0), pad);
  const Token& b1 = state.getToken(state.buffer(1), pad);
  const Token& b2 = state.getToken(state.buffer(2), pad);
  const Token& b3 = state.getToken(state.buffer(3), pad);

  const Token& lc1_s0 = state.getToken(state.leftmost(s0.id), pad);
  const Token& rc1_s0 = state.getToken(state.rightmost(s0.id), pad);
  const Token& lc2_s0 =
      state.getToken(state.leftmost(s0.id, lc1_s0.id + 1), pad);
  const Token& rc2_s0 =
      state.getToken(state.rightmost(s0.id, rc1_s0.id - 1), pad);
  const Token& lc1_s1 = state.getToken(state.leftmost(s1.id), pad);
  const Token& rc1_s1 = state.getToken(state.rightmost(s1.id), pad);
  const Token& lc2_s1 =
      state.getToken(state.leftmost(s1.id, lc1_s1.id + 1), pad);
  const Token& rc2_s1 =
      state.getToken(state.rightmost(s1.id, rc1_s1.id - 1), pad);

  const Token& lc1_lc1_s0 = state.getToken(state.leftmost(lc1_s0.id), pad);
  const Token& rc1_rc1_s0 = state.getToken(state.rightmost(rc1_s0.id), pad);
  const Token& lc1_lc1_s1 = state.getToken(state.leftmost(lc1_s1.id), pad);
  const Token& rc1_rc1_s1 = state.getToken(state.rightmost(rc1_s1.id), pad);

  return {
      // word features
      s0.word,
      s1.word,
      s2.word,
      s3.word,
      b0.word,
      b1.word,
      b2.word,
      b3.word,
      lc1_s0.word,
      rc1_s0.word,
      lc2_s0.word,
      rc2_s0.word,
      lc1_s1.word,
      rc1_s1.word,
      lc2_s1.word,
      rc2_s1.word,
      lc1_lc1_s0.word,
      rc1_rc1_s0.word,
      lc1_lc1_s1.word,
      rc1_rc1_s1.word,
      // pos features
      s0.tag,
      s1.tag,
      s2.tag,
      s3.tag,
      b0.tag,
      b1.tag,
      b2.tag,
      b3.tag,
      lc1_s0.tag,
      rc1_s0.tag,
      lc2_s0.tag,
      rc2_s0.tag,
      lc1_s1.tag,
      rc1_s1.tag,
      lc2_s1.tag,
      rc2_s1.tag,
      lc1_lc1_s0.tag,
      rc1_rc1_s0.tag,
      lc1_lc1_s1.tag,
      rc1_rc1_s1.tag,
      // label features
      lc1_s0.id != -1 ? (unsigned) state.label(lc1_s0.id) : pad.label,
      rc1_s0.id != -1 ? (unsigned) state.label(rc1_s0.id) : pad.label,
      lc2_s0.id != -1 ? (unsigned) state.label(lc2_s0.id) : pad.label,
      rc2_s0.id != -1 ? (unsigned) state.label(rc2_s0.id) : pad.label,
      lc1_s1.id != -1 ? (unsigned) state.label(lc1_s1.id) : pad.label,
      rc1_s1.id != -1 ? (unsigned) state.label(rc1_s1.id) : pad.label,
      lc2_s1.id != -1 ? (unsigned) state.label(lc2_s1.id) : pad.label,
      rc2_s1.id != -1 ? (unsigned) state.label(rc2_s1.id) : pad.label,
      lc1_lc1_s0.id != -1 ? (unsigned) state.label(lc1_lc1_s0.id) : pad.label,
      rc1_rc1_s0.id != -1 ? (unsigned) state.label(rc1_rc1_s0.id) : pad.label,
      lc1_lc1_s1.id != -1 ? (unsigned) state.label(lc1_lc1_s1.id) : pad.label,
      rc1_rc1_s1.id != -1 ? (unsigned) state.label(rc1_rc1_s1.id) : pad.label,
  };
}

std::vector<std::vector<FeatureVector>> Feature::unpackFeatures(
    const std::vector<FeatureVector>& features) {
  size_t batch_size = features.size();
  int index = 0;

  int end = kNWordFeatures;
  std::vector<FeatureVector> word_features;
  word_features.reserve(kNWordFeatures);
  for (; index < end; ++index) {
    FeatureVector feature_batch(batch_size);
    for (unsigned i = 0; i < batch_size; ++i) {
      feature_batch[i] = features[i][index];
    }
    word_features.push_back(std::move(feature_batch));
  }
  end += kNPosFeatures;
  std::vector<FeatureVector> pos_features;
  pos_features.reserve(kNPosFeatures);
  for (; index < end; ++index) {
    FeatureVector feature_batch(batch_size);
    for (unsigned i = 0; i < batch_size; ++i) {
      feature_batch[i] = features[i][index];
    }
    pos_features.push_back(std::move(feature_batch));
  }
  end += kNLabelFeatures;
  std::vector<FeatureVector> label_features;
  label_features.reserve(kNLabelFeatures);
  for (; index < end; ++index) {
    FeatureVector feature_batch(batch_size);
    for (unsigned i = 0; i < batch_size; ++i) {
      feature_batch[i] = features[i][index];
    }
    label_features.push_back(std::move(feature_batch));
  }
  return {word_features, pos_features, label_features};
}

}  // namespace transitionparser
