//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/token.h"

namespace coordparser {

// cppcheck-suppress uninitMemberVar
Token::Token(const std::vector<string>& attributes)
    : Token(attributes[0], attributes[1], attributes[4], attributes[6],
            attributes[7]) {}

// cppcheck-suppress uninitMemberVar
Token::Token(const string& id, const string& form, const string& lemma,
             const string& cpostag, const string& postag, const string& feats,
             const string& head, const string& deprel, const string& phead,
             const string& pdeprel)
    : Token(id, form, postag, head, deprel) {}

Token::Token(const Token& token)
    : id(token.id), form(token.form), postag(token.postag),
      head(token.head), deprel(token.deprel) {
}

Token::Token(Token &&token) noexcept
    : id(token.id), form(token.form), postag(token.postag),
      head(token.head), deprel(token.deprel) {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << Token::convert(Token::Attribute::FORM, token.form);
  return os;
}

const Token& Token::createRoot() {
  return root;
}

const Token& Token::createPad() {
  return pad;
}

const Token::Dict& Token::getDict(const Token::Attribute name) {
  return attribute_dicts_[name];
}

void Token::fixDictionaries() {
  for (auto& pair : attribute_dicts_) {
    pair.second.fix();
  }
}

Token::Token(const int id, const int form, const int postag,
             const int head, const int deprel)
    : id(id), form(form), postag(postag), head(head), deprel(deprel) {}

Token::Token(const string& id, const string& form, const string& postag,
             const string& head, const string& deprel)
    : Token(
        std::stoi(id),
        convert(Token::Attribute::FORM,   form),
        convert(Token::Attribute::POSTAG, postag),
        std::stoi(head),
        convert(Token::Attribute::DEPREL, deprel)
    ) {}

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

const Token Token::root(
    "0",       // ID
    "<ROOT>",  // FORM
    // "<ROOT>",  // LEMMA
    // "ROOT",    // CPOSTAG
    "ROOT",    // POSTAG
    // "_",       // FEATS
    "-1",      // HEAD
    "ROOT");   // DEPREL
// "_",       // PHEAD
// "_");      // PDEPREL

const Token Token::pad(
    "-10",    // ID
    "<PAD>",  // FORM
    // "<PAD>",  // LEMMA
    // "NULL",   // CPOSTAG
    "NULL",   // POSTAG
    // "",       // FEATS
    "-11",     // HEAD
    "NULL");  // DEPREL
// "",       // PHEAD
// "");      // PDEPREL

}  // namespace coordparser
