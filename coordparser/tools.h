//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_TOOLS_H_
#define COORDPARSER_TOOLS_H_

#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "coordparser/sentence.h"
#include "coordparser/utility.h"

namespace coordparser {

std::vector<Sentence> read_conll(const std::string& filepath) {
  std::ifstream ifs(filepath);
  if (ifs.fail()) {
    // @TODO
  }

  std::vector<Sentence> sentences;
  std::vector<Token> tokens;
  tokens.push_back(std::move(Token::createRoot()));
  std::string line;
  int count = 0;

  while (getline(ifs, line)) {
    utility::string::trim(line);
    if (line.length() == 0) {
      if (tokens.size() > 1) {
        sentences.emplace_back(++count, tokens);
        tokens.clear();
        tokens.push_back(std::move(Token::createRoot()));
      }
    } else {
      tokens.emplace_back(utility::string::split(line, '\t'));
    }
  }
  if (tokens.size() > 1) {
    sentences.emplace_back(++count, tokens);
  }
  return sentences;
}

template<typename T>
std::vector<std::vector<T>> create_batch(const std::vector<T>& samples,
                                         const size_t batch_size,
                                         const bool shuffle = true) {
  size_t sample_size = samples.size();
  size_t num_batches = sample_size / batch_size + 1;

  std::vector<int> indices(sample_size);
  std::iota(std::begin(indices), std::end(indices), 0);
  if (shuffle) std::shuffle(indices.begin(), indices.end(), std::mt19937());

  std::vector<std::vector<T>> batches;
  batches.reserve(num_batches);

  for (int batch_index = 0; batch_index < num_batches; ++batch_index) {
    int offset = batch_index * batch_size;
    const size_t current_batch_size =
        std::min(sample_size - offset, batch_size);
    std::vector<T> batch(current_batch_size);
    for (int index = 0; index < current_batch_size; ++index) {
      batch[index] = samples[indices[offset + index]];
    }
    batches.push_back(std::move(batch));
  }

  return batches;
}

template<typename X, typename Y>
std::vector<std::pair<std::vector<X>, std::vector<Y>>>
create_batch(std::vector<X> samples_x,
             std::vector<Y> samples_y,
             const size_t batch_size,
             const bool shuffle = true) {
  size_t sample_size = samples_x.size();
  COORDPARSER_ASSERT(sample_size = samples_y.size(),
                     "samples_x and samples_y must be same size");
  size_t num_batches = sample_size / batch_size + 1;

  std::vector<int> indices(sample_size);
  std::iota(std::begin(indices), std::end(indices), 0);
  if (shuffle) std::shuffle(indices.begin(), indices.end(), std::mt19937());

  std::vector<std::pair<std::vector<X>, std::vector<Y>>> batches;
  batches.reserve(num_batches);

  for (unsigned batch_index = 0; batch_index < num_batches; ++batch_index) {
    int offset = batch_index * batch_size;
    const size_t current_batch_size =
        std::min(sample_size - offset, batch_size);
    std::vector<X> batch_x(current_batch_size);
    std::vector<Y> batch_y(current_batch_size);
    for (unsigned index = 0; index < current_batch_size; ++index) {
      batch_x[index] = samples_x[indices[offset + index]];
      batch_y[index] = samples_y[indices[offset + index]];
    }
    batches.emplace_back(std::move(batch_x), std::move(batch_y));
  }

  return batches;
}

}  // namespace coordparser

#endif  // COORDPARSER_TOOLS_H_
