//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_TOKEN_H
#define COORDPARSER_TOKEN_H

#include <string>
#include <vector>
#include <boost/bimap.hpp>
#include <iostream>

using std::string;

namespace coordparser {

class Token {

public:
  const int id_;
  const int form_;
  // const int lemma_;
  // const int cpostag_;
  const int postag_;
  // const int feats;
  const int head_;
  const int deprel_;
  // const int phead_;
  // const int pdeprel_;

  enum Attribute {
      ID,
      FORM,
      // LEMMA,
      // CPOSTAG,
      POSTAG,
      // FEATS,
      HEAD,
      DEPREL,
      // PHEAD,
      // PDEPREL,
  };

  Token() = delete;

  explicit Token(const std::vector<string>& attributes);

  Token(const string& id, const string& form, const string& lemma, const string& cpostag, const string& postag,
        const string& feats, const string& head, const string& deprel, const string& phead, const string& pdeprel);

  Token(const Token& token);

  Token(Token&& token) noexcept;

  std::string getForm() const;

  std::string getPostag() const;

  std::string getDeprel() const;

  static Token createRoot();

  friend std::ostream& operator<<(std::ostream& os, const Token& token);

private:
  typedef boost::bimap<std::pair<Attribute, std::string>, int> attribute_map;

  static attribute_map attributes_map_;

  Token(const int id, const int form, const int postag, const int head, const int deprel);

  Token(const string& id, const string& form, const string& postag, const string& head, const string& deprel);

  static int registerAttribute(const Attribute name, const string& value);

  static std::string getAttribute(const int index);
};

}


#endif //COORDPARSER_TOKEN_H
