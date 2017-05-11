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
        "/Users/hiroki/Desktop/NLP/data/TreeBank3_stanford/dep/wsj_02.conll";
    ConllReader reader(filepath);

    dynet::Model model;
    std::unique_ptr<dynet::Trainer> optimizer =
        std::make_unique<dynet::SimpleSGDTrainer>(model);

    std::shared_ptr<NeuralClassifier> classifier
        = std::make_shared<MlpClassifier>(
            model,
            10000,  // Token::getVocabSize(),
            64,
            Feature::kNWordFeatures,
            10000,  // Token::getVocabSize(),
            64,
            Feature::kNPosFeatures,
            10000,  // Token::getVocabSize(),
            64,
            Feature::kNLabelFeatures,
            1024,
            256,
            48);

    const std::vector<Sentence> sentences = reader.read();

    std::vector<Feature> X;
    std::vector<unsigned> Y;

    for (auto& sentence : sentences) {
      std::shared_ptr<State> state = std::make_shared<State>(&sentence);
      while (!state->isTerminal()) {
        Action action = Transition::getOracle(*state);
        Y.push_back(static_cast<unsigned>(action));
        X.emplace_back(*state);
        Transition::apply(action, state);
      }
    }

    int epoch = 0;
    int num_epochs = 10;
    uint64_t size = X.size();

    while (epoch < num_epochs) {
      log::info("iteration {}", epoch + 1);
      double loss = 0;
      for (int i = 0; i < size; i++) {
        log::info("process sample {} of {}", i, size);
        dynet::ComputationGraph cg;
        classifier->prepare(&cg);
        Feature x = X[i];
        unsigned t = Y[i];
        auto y = classifier->run(x.getWordFeatures(),
                                 x.getPosFeatures(),
                                 x.getLabelFeatures());
        auto loss_expr = dynet::expr::pickneglogsoftmax(y, t);
        loss += dynet::as_scalar(cg.forward(loss_expr));
        cg.backward(loss_expr);
      }
      log::info("loss {}", loss);
      ++epoch;
    }
  }

 private:
  void initialize() {
    dynet::DynetParams params;
    params.random_seed = 818426556;

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
