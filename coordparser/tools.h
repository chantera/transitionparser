//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_TOOLS_H_
#define COORDPARSER_TOOLS_H_

#include <boost/archive/text_oarchive.hpp>

#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "coordparser/sentence.h"
#include "coordparser/utility.h"

namespace coordparser {

namespace tools {

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
        if (sentences.size() >= 999) break;
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

class CmdArgs {
 public:
  CmdArgs(int argc, const char* argv[]) : argc_(argc), argv_(argv) {
    parse();
  }

  friend std::ostream& operator<<(std::ostream& os, const CmdArgs& args) {
    os << "params: ";
    int i;
    int size = args.params_.size();
    for (i = 0; i < size; ++i) {
      os << args.params_[i];
      if (i != size - 1) {
        os << ", ";
      }
    }
    os << ", options: ";
    size = args.option_keys_.size();
    for (i = 0; i < size; ++i) {
      auto& key = args.option_keys_[i];
      auto& value = args.options_.at(key);
      os << key << "=" << value;
      if (i != size - 1) {
        os << ", ";
      }
    }
    return os;
  }

  // regex
  //     ^-(-)?([^=\s]+)(?:[=\s](.*))?$        1   2       3
  // long name pattern
  //     --long1=value1      long1 => value1   "-" "long1" "value1"
  //     --long2 value2      long2 => value2   "-" "long2" "value1"
  //     --long3             long3 => ""       "-" "long3" ""
  // short name pattern
  //     -a=value4           a     => value4   ""  "a"     "value4"
  //     -b value5           b     => value5   ""  "b"     "value5"
  //     -c                  c     => ""       ""  "c"     ""
  //     -def=value6         d     => ""       ""  "def"   "value6"
  //                         e     => ""       ""  "def"   "value6"
  //                         f     => value6   ""  "def"   "value6"
  //     -ghi value7         g     => ""       ""  "ghi"   "value7"
  //                         h     => ""       ""  "ghi"   "value7"
  //                         i     => value7   ""  "ghi"   "value7"
  //     -jkl                j     => ""       ""  "jkl"   ""
  //                         k     => ""       ""  "jkl"   ""
  //                         l     => ""       ""  "jkl"   ""
  void parse() {
    if (argc_ < 2) {
      return;
    }
    std::regex re("^-(-)?([^=\\s]+)(?:[=\\s](.*))?$");
    std::cmatch matches;
    for (int i = 1; i < argc_; ++i) {
      if (!std::regex_match(argv_[i], matches, re)) {
        params_.emplace_back(argv_[i]);
        continue;
      }
      std::vector<std::string> names;
      if (matches[1] == '-') {
        names.emplace_back(matches[2]);
      } else {
        for (char& ch : matches[2].str()) {
          names.push_back(std::string({ch}));
        }
      }
      std::string name = std::move(names.back());
      names.pop_back();
      for (auto& n : names) {
        add_option(n, "");
      }
      if (matches[3].str() != "") {
        add_option(name, matches[3]);
      } else if (i + 1 < argc_) {
        const char* next = argv_[i + 1];
        if (next[0] != '-') {
          add_option(name, next);
          i++;
        } else {
          add_option(name, "");
        }
      } else {
        add_option(name, "");
      }
    }
  }

  unsigned getParamSize() {
    return static_cast<unsigned>(params_.size());
  }

  std::string getParam(unsigned index) {
    return params_.at(index);
  }

  std::string getParamOrDefault(unsigned index,
                                const std::string& defaultValue) {
    if (index > params_.size()) {
      return defaultValue;
    }
    return params_[index];
  }

  bool hasOption(const std::string& key) {
    return !(options_.find(key) == options_.end());
  }

  const std::unordered_map<std::string, std::string>& getOptions() {
    return options_;
  }

  std::string getOption(const std::string& key) {
    if (!hasOption(key)) {
      COORDPARSER_EXCEPTION("cannot retrieve the option [key={}]", key);
    }
    return options_.at(key);
  }

  std::string getOptionOrDefault(const std::string& key,
                                 const std::string& defaultValue) {
    if (!hasOption(key)) {
      return defaultValue;
    }
    return options_[key];
  }

  int size() {
    return argc_;
  }

 private:
  void add_option(const std::string& key, const std::string& value) {
    option_keys_.push_back(key);
    options_[key] = value;
  }

  const int argc_;
  const char** argv_;
  std::vector<std::string> params_;
  std::vector<std::string> option_keys_;
  std::unordered_map<std::string, std::string> options_;
};

template<class T>
void archive(const std::string& filepath, const T& object) {
  std::ofstream ofs(filepath);
  boost::archive::text_oarchive oar(ofs);
  oar << object;
}

}  // namespace tools

}  // namespace coordparser

#endif  // COORDPARSER_TOOLS_H_
