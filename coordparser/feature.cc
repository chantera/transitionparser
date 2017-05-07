//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/feature.h"

namespace coordparser {

const unsigned Feature::kNWordFeatures  = 20;
const unsigned Feature::kNPosFeatures   = 20;
const unsigned Feature::kNLabelFeatures = 12;

Feature::Feature(const State &state) :
  word_features_(kNWordFeatures),
  pos_features_(kNPosFeatures),
  label_features_(kNLabelFeatures) {
  Token pad = Token::createPad();
  const Token& s0 = state.getStackToken(0, pad);
  const Token& s1 = state.getStackToken(1, pad);
  const Token& s2 = state.getStackToken(2, pad);
  const Token& s3 = state.getStackToken(3, pad);
  const Token& b0 = state.getBufferToken(0, pad);
  const Token& b1 = state.getBufferToken(1, pad);
  const Token& b2 = state.getBufferToken(2, pad);
  const Token& b3 = state.getBufferToken(3, pad);

  const Token& lc1_s0 = state.getLeftmostToken(s0.id_, pad);
  const Token& rc1_s0 = state.getRightmostToken(s0.id_, pad);
  const Token& lc2_s0 = state.getLeftmostToken(s0.id_, pad, lc1_s0.id_ + 1);
  const Token& rc2_s0 = state.getRightmostToken(s0.id_, pad, rc1_s0.id_ - 1);
  const Token& lc1_s1 = state.getLeftmostToken(s1.id_, pad);
  const Token& rc1_s1 = state.getRightmostToken(s1.id_, pad);
  const Token& lc2_s1 = state.getLeftmostToken(s1.id_, pad, lc1_s1.id_ + 1);
  const Token& rc2_s1 = state.getRightmostToken(s1.id_, pad, rc1_s1.id_ - 1);

  const Token& lc1_lc1_s0 = state.getLeftmostToken(lc1_s0.id_, pad);
  const Token& rc1_rc1_s0 = state.getLeftmostToken(rc1_s0.id_, pad);
  const Token& lc1_lc1_s1 = state.getLeftmostToken(lc1_s1.id_, pad);
  const Token& rc1_rc1_s1 = state.getLeftmostToken(rc1_s1.id_, pad);

  word_features_ = {
      (unsigned) s0.form_,
      (unsigned) s1.form_,
      (unsigned) s2.form_,
      (unsigned) s3.form_,
      (unsigned) b0.form_,
      (unsigned) b1.form_,
      (unsigned) b2.form_,
      (unsigned) b3.form_,
      (unsigned) lc1_s0.form_,
      (unsigned) rc1_s0.form_,
      (unsigned) lc2_s0.form_,
      (unsigned) rc2_s0.form_,
      (unsigned) lc1_s1.form_,
      (unsigned) rc1_s1.form_,
      (unsigned) lc2_s1.form_,
      (unsigned) rc2_s1.form_,
      (unsigned) lc1_lc1_s0.form_,
      (unsigned) rc1_rc1_s0.form_,
      (unsigned) lc1_lc1_s1.form_,
      (unsigned) rc1_rc1_s1.form_,
  };

  pos_features_ = {
      (unsigned) s0.postag_,
      (unsigned) s1.postag_,
      (unsigned) s2.postag_,
      (unsigned) s3.postag_,
      (unsigned) b0.postag_,
      (unsigned) b1.postag_,
      (unsigned) b2.postag_,
      (unsigned) b3.postag_,
      (unsigned) lc1_s0.postag_,
      (unsigned) rc1_s0.postag_,
      (unsigned) lc2_s0.postag_,
      (unsigned) rc2_s0.postag_,
      (unsigned) lc1_s1.postag_,
      (unsigned) rc1_s1.postag_,
      (unsigned) lc2_s1.postag_,
      (unsigned) rc2_s1.postag_,
      (unsigned) lc1_lc1_s0.postag_,
      (unsigned) rc1_rc1_s0.postag_,
      (unsigned) lc1_lc1_s1.postag_,
      (unsigned) rc1_rc1_s1.postag_,
  };

  label_features_ = {
      (unsigned) lc1_s0.deprel_,
      (unsigned) rc1_s0.deprel_,
      (unsigned) lc2_s0.deprel_,
      (unsigned) rc2_s0.deprel_,
      (unsigned) lc1_s1.deprel_,
      (unsigned) rc1_s1.deprel_,
      (unsigned) lc2_s1.deprel_,
      (unsigned) rc2_s1.deprel_,
      (unsigned) lc1_lc1_s0.deprel_,
      (unsigned) rc1_rc1_s0.deprel_,
      (unsigned) lc1_lc1_s1.deprel_,
      (unsigned) rc1_rc1_s1.deprel_,
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
