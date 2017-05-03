//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_FEATURE_H_
#define COORDPARSER_FEATURE_H_

#include <vector>

#include "coordparser/state.h"

namespace coordparser {

class State;

class Feature {
 public:
  static const unsigned kNWordFeatures  = 20;
  static const unsigned kNPosFeatures   = 20;
  static const unsigned kNLabelFeatures = 12;

  Feature() = delete;
  explicit Feature(const State &state);

  const std::vector<unsigned>& getWordFeatures() const;

  const std::vector<unsigned>& getPosFeatures() const;

  const std::vector<unsigned>& getLabelFeatures() const;

 private:
  std::vector<unsigned> word_features_;
  std::vector<unsigned> pos_features_;
  std::vector<unsigned> label_features_;
};

}  // namespace coordparser

#endif  // COORDPARSER_FEATURE_H_