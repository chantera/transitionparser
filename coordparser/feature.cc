//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/feature.h"

namespace coordparser {

Feature::Feature(const State &state) :
  word_features_(kNWordFeatures),
  pos_features_(kNPosFeatures),
  label_features_(kNLabelFeatures) {
  const Token& s0 = state.getStackToken(0);
  const Token& s1 = state.getStackToken(1);
  const Token& s2 = state.getStackToken(2);
  const Token& s3 = state.getStackToken(3);

  const Token& b0 = state.getBufferToken(0);
  const Token& b1 = state.getBufferToken(1);
  const Token& b2 = state.getBufferToken(2);
  const Token& b3 = state.getBufferToken(3);

  word_features_ = {
      (unsigned) s0.form_,
      (unsigned) s1.form_,
      (unsigned) s2.form_,
      (unsigned) s3.form_,
      (unsigned) b0.form_,
      (unsigned) b1.form_,
      (unsigned) b2.form_,
      (unsigned) b3.form_
  };
}

const std::vector<unsigned>& Feature::getWordFeatures() const {
  return word_features_;
}

const std::vector<unsigned>& Feature::getPosFeatures() const {
  return pos_features_;
}

const std::vector<unsigned>& Feature::getLabelFeatures() const {
  return label_features_;
}

}  // namespace coordparser
