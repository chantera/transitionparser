//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_FEATURE_H_
#define COORDPARSER_FEATURE_H_

#include <vector>

#include "coordparser/state.h"
#include "coordparser/utility.h"

namespace coordparser {

class State;

typedef std::vector<unsigned> FeatureVector;

class Feature {
 public:
  static const unsigned kNWordFeatures;
  static const unsigned kNPosFeatures;
  static const unsigned kNLabelFeatures;

  static FeatureVector extract(const State &state);
  static std::vector<std::vector<FeatureVector>> unpackFeatures(
      const std::vector<FeatureVector>& features);

 private:
  Feature() = delete;
  DISALLOW_COPY_AND_MOVE(Feature);
  ~Feature() = delete;
};

}  // namespace coordparser

#endif  // COORDPARSER_FEATURE_H_
