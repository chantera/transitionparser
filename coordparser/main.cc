//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include <dynet/dynet.h>
#include <dynet/expr.h>
#include <dynet/tensor.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "coordparser/classifier.h"
#include "coordparser/logger.h"
#include "coordparser/parser.h"
#include "coordparser/reader.h"

namespace coordparser {

class App {
 public:
  App() {
    initialize();
  }

  void train() {
    log::info("Hello, World!");

    std::string filepath =
        "/Users/hiroki/Desktop/NLP/data/archive.20161120/"
            "penn_treebank/dep/stanford/section/parse-train.conll";
    ConllReader reader(filepath);
    const std::vector<Sentence> sentences = reader.read();
    Token::fixDictionaries();

    dynet::Model model;
    auto optimizer = dynet::SimpleSGDTrainer(model);

    std::shared_ptr<NeuralClassifier> classifier
        = std::make_shared<MlpClassifier>(
            model,
            Token::getDict(Token::FORM).size(),
            64,
            Feature::kNWordFeatures,
            Token::getDict(Token::POSTAG).size(),
            64,
            Feature::kNPosFeatures,
            Token::getDict(Token::DEPREL).size(),
            64,
            Feature::kNLabelFeatures,
            1024,
            256,
            (Token::getDict(Token::DEPREL).size() - 2) * 2 + 1);

    GreedyParser parser(classifier);

    std::vector<FeatureVector> X;
    std::vector<unsigned> Y;

    for (auto& sentence : sentences) {
      State state(sentence);
      while (!Transition::isTerminal(state)) {
        Action action = Transition::getOracle(state);
        Y.push_back(static_cast<unsigned>(action));
        X.push_back(std::move(Feature::extract(state)));
        Transition::apply(action, &state);
      }
    }

    int epoch = 0;
    int num_epochs = 10;
    uint64_t batch_size = 1024;
    uint64_t size = X.size();
    uint64_t num_batches = size / batch_size + 1;

    while (epoch < num_epochs) {
      log::info("iteration {}", epoch + 1);
      double loss = 0;
      double correct = 0;

      for (int batch_index = 0; batch_index < num_batches; ++batch_index) {
        log::info("process batch {} of {}", batch_index + 1, num_batches);

        int offset = batch_index * batch_size;
        const uint64_t current_batch_size = std::min(size - offset, batch_size);
        std::vector<FeatureVector> x(current_batch_size);
        std::vector<unsigned> t(current_batch_size);
        int index;
        for (index = 0; index < current_batch_size; ++index) {
          x[index] = X[offset + index];
          t[index] = Y[offset + index];
        }

        dynet::ComputationGraph cg;
        classifier->prepare(&cg);

        auto y = classifier->run(x);
        auto pred_actions = dynet::as_vector(dynet::TensorTools::argmax(
            cg.incremental_forward(dynet::expr::softmax(y))));
        for (index = 0; index < current_batch_size; ++index) {
          if (pred_actions[index] == t[index]) ++correct;
        }
        auto loss_expr = dynet::expr::sum_batches(
            dynet::expr::pickneglogsoftmax(y, t)) / current_batch_size;
        loss += dynet::as_scalar(cg.incremental_forward(loss_expr));
        cg.backward(loss_expr);
        optimizer.update();
      }
      log::info("loss {}", loss);
      log::info("accuracy {}", correct / size);
      ++epoch;

      // if (epoch < 6) {
      //   continue;
      // }

      // float count = 0;
      // float uas = 0;
      // float las = 0;
      // int j = 0;
      // int num_sentences = sentences.size();
      // for (auto& sentence : sentences) {
      //   log::info("testing sentence {} of {}", ++j, num_sentences);
      //   dynet::ComputationGraph cg;
      //   classifier->prepare(&cg);
      //   State state = parser.parse(sentence);
      //   /*
      //   log::warning("scores: {}", classifier->compute(Feature::extract(*state)));
      //   log::debug("step: {}", state->step_);
      //    */
      //   log::debug("{}", state->heads_);
      //   log::debug("{}", state->labels_);
      //   for (auto token : sentence.tokens) {
      //     if (token.id == 0) continue;

      //     ++count;
      //     if (state->heads_[token.id] == token.head) {
      //       uas += 1;
      //       if (state->labels_[token.id] == token.label) {
      //         las += 1;
      //       }
      //     }
      //   }
      // }

      // log::info("UAS: {:.4f}, LAS: {:.4f}",
      //           (uas / count) * 100,
      //           (las / count) * 100);
    }
  }

 private:
  void initialize() {
    dynet::DynetParams params;
    params.random_seed = 818426556;
    params.mem_descriptor = "512,512,1024";
    params.random_seed = 0;
    params.weight_decay = 0.0f;       // L2正則化の強さ
    params.shared_parameters = false; // パラメータをshared memory上に置くかどうか
    dynet::initialize(params);

    coordparser::AppLogger::init("/Users/hiroki/work/coordparser/logs/test.log",
                    log::LogLevel::off,
                    log::LogLevel::debug);
  }
};

}  // namespace coordparser

int main() {
  coordparser::App app;
  app.train();
  return 0;
}
