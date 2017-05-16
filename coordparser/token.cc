//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/token.h"

namespace coordparser {

// cppcheck-suppress uninitMemberVar
Token::Token(const std::vector<string>& attributes)
    : Token(std::stoi(attributes[0]), attributes[1], attributes[4],
            std::stoi(attributes[6]), attributes[7]) {}

// cppcheck-suppress uninitMemberVar
Token::Token(const string& id, const string& form, const string& lemma,
             const string& cpostag, const string& postag, const string& feats,
             const string& head, const string& deprel, const string& phead,
             const string& pdeprel)
    : Token(std::stoi(id), form, postag, std::stoi(head), deprel) {}

Token::Token(const Token& token)
    : id(token.id), form(token.form), postag(token.postag),
      head(token.head), deprel(token.deprel), word(token.word),
      tag(token.tag), label(token.label) {}

Token::Token(Token &&token) noexcept
    : id(token.id), form(token.form), postag(token.postag),
      head(token.head), deprel(token.deprel), word(token.word),
      tag(token.tag), label(token.label) {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << token.form;
  return os;
}

const Token& Token::createRoot() {
  static Token root(
      0,         // ID
      "<ROOT>",  // FORM
      // "<ROOT>",  // LEMMA
      // "<ROOT>",  // CPOSTAG
      "<ROOT>",  // POSTAG
      // "_",       // FEATS
      0,        // HEAD
      "root");   // DEPREL
// "_",       // PHEAD
// "_");      // PDEPREL
  return root;
}

const Token& Token::createPad() {
  static Token pad(
      -1,       // ID
      "<PAD>",  // FORM
      // "<PAD>",  // LEMMA
      // "<PAD>",  // CPOSTAG
      "<PAD>",  // POSTAG
      // "",       // FEATS
      -1,       // HEAD
      "<PAD>");   // DEPREL
// "",       // PHEAD
// "");      // PDEPREL
  return pad;
}

const Token::Dict& Token::getDict(const Token::Attribute name) {
  return attribute_dicts_[name];
}

void Token::fixDictionaries() {
  for (auto& pair : attribute_dicts_) {
    pair.second.lookup("<PAD>");
    pair.second.set_unk("<UNKNOWN>");
    pair.second.fix();
  }
}

Token::Token(const int& id, const string& form, const string& postag,
             const int& head, const string& deprel)
    : id(id),
      form(form),
      postag(postag),
      head(head),
      deprel(deprel),
      word(convert(Token::Attribute::FORM, form)),
      tag(convert(Token::Attribute::POSTAG, postag)),
      label(convert(Token::Attribute::DEPREL, deprel)) {}

int Token::convert(const Token::Attribute name, const string& value) {
  return attribute_dicts_[name].lookup(value);
}

std::string Token::convert(const Token::Attribute name, const int index) {
  return attribute_dicts_[name].lookup(index);
}

std::unordered_map<Token::Attribute, Token::Dict> Token::attribute_dicts_ = {
    {Token::Attribute::FORM,   Dict()},
    {Token::Attribute::POSTAG, Dict()},
    {Token::Attribute::DEPREL, Dict()},
};

}  // namespace coordparser
