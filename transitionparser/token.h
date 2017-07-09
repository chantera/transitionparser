//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef TRANSITIONPARSER_TOKEN_H_
#define TRANSITIONPARSER_TOKEN_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;

namespace transitionparser {

struct Token {
 public:
  enum Attribute {
    ID = 0,
    FORM = 1,
    // LEMMA = 2,
    // CPOSTAG = 3,
    POSTAG = 4,
    // FEATS = 5,
    HEAD = 6,
    DEPREL = 7,
    // PHEAD = 8,
    // PDEPREL = 9,
  };

  class Dict {
   public:
    Dict() : map_unk(false) {}
    inline int lookup(const std::string& word) {
      auto i = d_.find(word);
      if (i == d_.end()) {
        if (map_unk) {
          return unk_id;
        }
        words_.push_back(word);
        return d_[word] = words_.size() - 1;
      } else {
        return i->second;
      }
    }
    inline const std::string& lookup(const int& id) const {
      return words_.at(id);
    }
    inline bool contains(const std::string& words) const {
      return !(d_.find(words) == d_.end());
    }
    inline size_t size() const {
      return words_.size();
    }
    void fix() {
      map_unk = true;
    }
    void clear() {
      words_.clear();
      d_.clear();
    }
    inline int set_unk(const std::string& unk) {
      unk_id = lookup(unk);
      return unk_id;
    }

   private:
    bool map_unk;
    int unk_id = -1;
    std::vector<std::string> words_;
    std::unordered_map<std::string, int> d_;
  };

  Token() = delete;
  explicit Token(const std::vector<string>& attributes);
  Token(const string& id, const string& form, const string& lemma,
        const string& cpostag, const string& postag, const string& feats,
        const string& head, const string& deprel, const string& phead,
        const string& pdeprel);
  Token(const Token& token);
  Token& operator=(const Token&) = default;
  Token(Token&& token) noexcept;
  Token& operator=(Token&&) = default;
  ~Token() {}

  friend std::ostream& operator<<(std::ostream& os, const Token& token);

  static const Token& createRoot();

  static const Token& createPad();

  static const Dict& getDict(const Attribute name);

  static void fixDictionaries();

  const int id;
  const std::string form;
  // const int lemma;
  // const int cpostag;
  const std::string postag;
  // const int feats;
  const int head;
  const std::string deprel;
  // const int phead;
  // const int pdeprel;

  const unsigned word;
  const unsigned tag;
  const unsigned label;

 private:
  inline Token(const int& id, const string& form, const string& postag,
        const int& head, const string& deprel);

  static inline int convert(const Attribute name, const string& value);

  static inline std::string convert(const Attribute name, const int index);

  static std::unordered_map<Attribute, Dict> attribute_dicts_;
};

}  // namespace transitionparser

#endif  // TRANSITIONPARSER_TOKEN_H_
