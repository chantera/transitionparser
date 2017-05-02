//
// Created by Hiroki Teranishi on 4/14/17.
//

#ifndef COORDPARSER_MLP_CLASSIFIER_H
#define COORDPARSER_MLP_CLASSIFIER_H

#include <dynet/expr.h>
#include <dynet/training.h>
#include "classifier.h"

namespace coordparser {

class MlpClassifier : public Classifier {

public:
  static const unsigned kNWordFeatures  = 20;
  static const unsigned kNPosFeatures   = 20;
  static const unsigned kNLabelFeatures = 12;

  MlpClassifier(const unsigned word_vocab_size,
                const unsigned word_embed_size,
                const unsigned pos_vocab_size,
                const unsigned pos_embed_size,
                const unsigned label_vocab_size,
                const unsigned label_embed_size,
                const unsigned hidden1_size,
                const unsigned hidden2_size,
                const unsigned n_labels);

  Action getNextAction(const State& state) override ;

private:
  typedef dynet::Trainer Optimizer;

  dynet::Model model_;
  std::unique_ptr<Optimizer> optimizer_;

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

#endif //COORDPARSER_MLP_CLASSIFIER_H
