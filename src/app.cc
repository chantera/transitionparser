//
// Created by Hiroki Teranishi on 3/2/17.
//

#include <iostream>
#include "app.h"
#include "logger.h"
#include "greedy_parser.h"
#include "conll_reader.h"

namespace coordparser {

App::App() {}

void App::initialize() {
  /*
  dynet::DynetParams params;

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
  const unsigned MAX_GPUS = 1024; // 適当に大きな値。挿さっているGPUの枚数以上にすればよい。
  params.ngpus_requested = false;
  params.ids_requested = false;
  params.requested_gpus = -1;
  params.gpus_mask = std::vector<int>(MAX_GPUS, 0);
#endif

  */

  // dynet::initialize(params);

}

void App::train() {
  Logger::log(Logger::LogLevel::INFO, "Hello, World!");

  std::string filepath = "/Users/hiroki/Desktop/NLP/data/TreeBank3_stanford/dep/wsj_02.conll";
  ConllReader reader(filepath);

  std::unique_ptr<Parser> parser(new GreedyParser());

  const std::vector<Sentence> sentences = reader.read();
  for (auto& sentence : sentences) {
      parser->parse(sentence);
  }
}

}