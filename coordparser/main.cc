//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include <dynet/dynet.h>
#include <dynet/expr.h>

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
            48);

    GreedyParser parser(classifier);

    std::vector<FeatureVector> X;
    std::vector<unsigned> Y;

    for (auto& sentence : sentences) {
      std::shared_ptr<State> state = std::make_shared<State>(&sentence);
      while (!state->isTerminal()) {
        Action action = Transition::getOracle(*state);
        Y.push_back(static_cast<unsigned>(action));
        X.push_back(std::move(Feature::extract(*state)));
        Transition::apply(action, state);
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

      for (int batch_index = 0; batch_index < num_batches; ++batch_index) {
        log::info("process batch {} of {}", batch_index + 1, num_batches);

        int offset = batch_index * batch_size;
        const uint64_t current_batch_size = std::min(size - offset, batch_size);
        std::vector<FeatureVector> x(current_batch_size);
        std::vector<unsigned> t(current_batch_size);
        for (int index = 0; index < current_batch_size; ++index) {
          x[index] = X[offset + index];
          t[index] = Y[offset + index];
        }

        dynet::ComputationGraph cg;
        classifier->prepare(&cg);

        auto y = classifier->run(x);
        auto loss_expr = dynet::expr::sum_batches(
            dynet::expr::pickneglogsoftmax(y, t)) / current_batch_size;
        loss += dynet::as_scalar(cg.forward(loss_expr));
        cg.backward(loss_expr);
        optimizer.update();
      }
      log::info("loss {}", loss);

      float count = 0;
      float uas = 0;
      float las = 0;
      int j = 0;
      int num_sentences = sentences.size();
      for (auto& sentence : sentences) {
        log::info("testing sentence {} of {}", ++j, num_sentences);
        dynet::ComputationGraph cg;
        classifier->prepare(&cg);
        std::shared_ptr<State> state = parser.parse(sentence);
        /*
        log::warning("scores: {}", classifier->compute(Feature::extract(*state)));
        log::debug("step: {}", state->step_);
         */
        log::debug("{}", state->heads_);
        log::debug("{}", state->labels_);
        for (auto token : sentence.tokens) {
          if (token.id == 0) continue;

          ++count;
          if (state->heads_[token.id] == token.head) {
            uas += 1;
            if (state->labels_[token.id] == token.deprel) {
              las += 1;
            }
          }
        }
      }

      log::info("UAS: {:.4f}, LAS: {:.4f}",
                (uas / count) * 100,
                (las / count) * 100);

      ++epoch;
    }
  }

 private:
  void initialize() {
    dynet::DynetParams params;
    params.random_seed = 818426556;
    params.mem_descriptor = "1024";

    /*
    // 確保するメモリ量を3つ指定する(MB単位)。
    // それぞれforward-path, backward-path, パラメータで使用する量。
    params.mem_descriptor = "1024,1024,2048";
    // 下記のように1つだけ指定することも可能。
    // その場合は内部的に3等分したものがそれぞれに使用される。
    //params.mem_descriptor = "4096";

    params.random_seed = 0;           // ランダムシード。0なら適当に初期化
    params.weight_decay = 0.0f;       // L2正則化の強さ
    params.shared_parameters = false; // パラメータをshared memory上に置くかどうか

  #if HAVE_CUDA
    // 以下はCUDAに関連付けてインストールした場合の設定。
    // 下記のように設定するとメモリに余裕があるGPUを1個勝手に選んで使う。
    // 複数GPUには今のところ未対応。
    const unsigned MAX_GPUS = 1024;
        // 適当に大きな値。挿さっているGPUの枚数以上にすればよい。
    params.ngpus_requested = false;
    params.ids_requested = false;
    params.requested_gpus = -1;
    params.gpus_mask = std::vector<int>(MAX_GPUS, 0);
  #endif
    */

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
