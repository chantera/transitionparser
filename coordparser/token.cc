//
// Created by Hiroki Teranishi on 4/7/17.
//

#include "token.h"

namespace coordparser {

Token::Token(Token &&token) noexcept
    : Token(token.id_, token.form_, token.postag_, token.head_, token.deprel_) {}

std::string Token::getForm() const {
  return Token::getAttribute(form_);
}

Token::Token(const std::vector<string>& attributes)
    : Token(attributes[0], attributes[1], attributes[4], attributes[6], attributes[7]) {}

Token::Token(const string& id, const string& form, const string& lemma, const string& cpostag, const string& postag,
             const string& feats, const string& head, const string& deprel, const string& phead, const string& pdeprel)
    : Token(id, form, postag, head, deprel) {}

Token::Token(const Token& token)
    : Token(token.id_, token.form_, token.postag_, token.head_, token.deprel_) {
  std::cout << id_ << token << std::endl;
}

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
      "ROOT"     // DEPREL
      // "_",       // PHEAD
      // "_"        // PDEPREL
  );
  return token;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << token.getForm();
  return os;
}

Token::attribute_map Token::attributes_map_ = [] {
  attribute_map map;
  return map;
}();

Token::Token(const int id, const int form, const int postag, const int head, const int deprel)
    : id_(id), form_(form), postag_(postag), head_(head), deprel_(deprel) {}

Token::Token(const string& id, const string& form, const string& postag, const string& head, const string& deprel)
    : Token(
        std::stoi(id),
        registerAttribute(Token::Attribute::FORM,   form  ),
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

}
