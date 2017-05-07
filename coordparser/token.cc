//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include "coordparser/token.h"

namespace coordparser {

std::string Token::getForm() const {
  return Token::getAttribute(form_);
}

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
    : id_(token.id_), form_(token.form_), postag_(token.postag_),
      head_(token.head_), deprel_(token.deprel_) {
  // std::cout << id_ << token << std::endl;
}

Token::Token(Token &&token) noexcept
    : id_(token.id_), form_(token.form_), postag_(token.postag_),
      head_(token.head_), deprel_(token.deprel_) {}

std::string Token::getPostag() const {
  return Token::getAttribute(postag_);
}

std::string Token::getDeprel() const {
  return Token::getAttribute(deprel_);
}

Token Token::createRoot() {
  Token token(
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
  return token;
}

Token Token::createPad() {
  static Token pad(
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
  return pad;
}

bool Token::isRoot() {
  return id_ == 0;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << token.getForm();
  return os;
}

Token::Token(const int id, const int form, const int postag,
             const int head, const int deprel)
    : id_(id), form_(form), postag_(postag), head_(head), deprel_(deprel) {}

Token::Token(const string& id, const string& form, const string& postag,
             const string& head, const string& deprel)
    : Token(
        std::stoi(id),
        registerAttribute(Token::Attribute::FORM,   form),
        registerAttribute(Token::Attribute::POSTAG, postag),
        std::stoi(head),
        registerAttribute(Token::Attribute::DEPREL, deprel)
    ) {}

int Token::registerAttribute(const Token::Attribute name, const string& value) {
  std::pair<Attribute, std::string> attribute(name, value);
  int index;
  if (attributes_map_.left.find(attribute) != attributes_map_.left.end()) {
    index = attributes_map_.left.at(attribute);
  } else {
    index = attributes_map_.size();
    attributes_map_.insert(attribute_map::value_type(attribute, index));
  }
  return index;
}

std::string Token::getAttribute(const int index) {
  if (attributes_map_.right.find(index) == attributes_map_.right.end()) {
    return nullptr;
  }
  return attributes_map_.right.at(index).second;
}

Token::attribute_map Token::attributes_map_ = [] {
  attribute_map map;
  return map;
}();

}  // namespace coordparser
