//
// Created by Hiroki Teranishi on 5/2/17.
//

#ifndef COORDPARSER_FEATURE_H
#define COORDPARSER_FEATURE_H

#include <vector>
#include "State.h"

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

}

#endif //COORDPARSER_FEATURE_H
