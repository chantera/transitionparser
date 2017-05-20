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
#include "coordparser/tools.h"

namespace coordparser {

class App {
 public:
  App() {}

  void train(const std::string& train_file,
             const std::string& test_file,
             const int num_epochs,
             const int batch_size) {
    log::info("Hello, World!");

    const std::vector<Sentence> train_sentences = read_conll(train_file);
    log::info("train sentence size: {} from '{}'",
              train_sentences.size(), train_file);
    Token::fixDictionaries();

    const std::vector<Sentence> test_sentences = read_conll(test_file);
    log::info("test sentence size: {} from '{}'",
              test_sentences.size(), test_file);

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
            Transition::numActions(Token::getDict(Token::DEPREL).size() - 2));

    GreedyParser parser(classifier);

    std::vector<FeatureVector> X;
    std::vector<unsigned> Y;

    for (auto& sentence : train_sentences) {
      State state(sentence);
      while (!Transition::isTerminal(state)) {
        Action action = Transition::getOracle(state);
        Y.push_back(static_cast<unsigned>(action));
        X.push_back(Feature::extract(state));
        Transition::apply(action, &state);
      }
    }

    int epoch = 0;
    size_t sample_size = X.size();
    size_t num_batches = sample_size / batch_size + 1;

    while (epoch < num_epochs) {
      log::info("iteration {}", epoch + 1);
      double loss = 0;
      double correct = 0;

      int batch_index = 0;
      for (auto& batch : create_batch(X, Y, batch_size, true)) {
        log::info("process batch {} of {}", ++batch_index, num_batches);
        auto& x = batch.first;
        auto& t = batch.second;
        size_t current_batch_size = x.size();

        dynet::ComputationGraph cg;
        classifier->prepare(&cg);

        auto y = classifier->run(x);
        auto pred_actions = dynet::as_vector(dynet::TensorTools::argmax(
            cg.incremental_forward(dynet::expr::softmax(y))));
        for (unsigned index = 0; index < current_batch_size; ++index) {
          if (pred_actions[index] == t[index]) ++correct;
        }
        auto loss_expr = dynet::expr::sum_batches(
            dynet::expr::pickneglogsoftmax(y, t)) / current_batch_size;
        loss += dynet::as_scalar(cg.incremental_forward(loss_expr));
        cg.backward(loss_expr);
        optimizer.update();
      }

      log::info("loss {}", loss);
      log::info("accuracy {}", correct / sample_size);
      ++epoch;

      if (epoch < 6) {
        continue;
      }

      float count = 0;
      float uas = 0;
      float las = 0;
      dynet::ComputationGraph cg;
      classifier->prepare(&cg);
      auto states = parser.parse_batch(test_sentences, batch_size);
      for (auto& state : states) {
        for (int i = 1; i < state->numTokens(); ++i) {
          ++count;
          if (state->head(i) == state->getToken(i).head) {
            uas += 1;
            if (state->label(i)
                == static_cast<int>(state->getToken(i).label)) {
              las += 1;
            }
          }
        }
      }
      log::info("UAS: {:.4f}, LAS: {:.4f}",
                (uas / count) * 100,
                (las / count) * 100);
    }
  }

  void initialize(unsigned random_seed = 0,
                  const std::string& memory = "512,1024,512",
                  log::LogLevel log_level = log::LogLevel::info,
                  log::LogLevel display_level = log::LogLevel::debug,
                  const std::string& log_dir = "logs") {
    dynet::DynetParams params;
    params.random_seed = random_seed;
    params.mem_descriptor = memory;
    params.weight_decay = 0.0f;
    params.shared_parameters = false;
    dynet::initialize(params);

    std::string file = log_dir + "/" + utility::date::strftime("%Y%m%d.log");
    coordparser::AppLogger::init(file, log_level, display_level);
  }
};

}  // namespace coordparser

int main(int argc, const char* argv[]) {
  utility::CmdArgs args(argc, argv);
  coordparser::App app;
  app.initialize(
      std::stoi(args.getOptionOrDefault("seed", "818426556")),
      args.getOptionOrDefault("memory", "512,1024,512"),
      coordparser::log::LogLevel::info,
      coordparser::log::LogLevel::debug,
      "/Users/hiroki/work/coordparser/logs");
  app.train(
      args.getOptionOrDefault(
          "trainfile",
          "/Users/hiroki/Desktop/NLP/data/archive.20161120/"
              "penn_treebank/dep/stanford/section/parse-train.conll"),
      args.getOptionOrDefault(
          "testfile",
          "/Users/hiroki/Desktop/NLP/data/archive.20161120/"
              "penn_treebank/dep/stanford/section/wsj_22.conll"),
      std::stoi(args.getOptionOrDefault("epoch", "10")),
      std::stoi(args.getOptionOrDefault("batchsize", "32")));
  return 0;
}
