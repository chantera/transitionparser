//
// Created by Hiroki Teranishi on 4/7/17.
//

#include "token.h"

namespace coordparser {

Token::Token(const std::vector<string>& attributes)
        : Token(attributes[0], attributes[1], attributes[4], attributes[6], attributes[7]) {}

Token::Token(const string& id, const string& form, const string& lemma, const string& cpostag, const string& postag,
             const string& feats, const string& head, const string& deprel, const string& phead, const string& pdeprel)
        : Token(id, form, postag, head, deprel) {}

std::string Token::getForm() const {
    return Token::getAttribute(form);
}

std::string Token::getPostag() const {
    return Token::getAttribute(postag);
}

std::string Token::getDeprel() const {
    return Token::getAttribute(deprel);
}

Token Token::createRoot() {
    Token token(
            // "0",       // ID
            // "<ROOT>",  // FORM
            // // "<ROOT>",  // LEMMA
            // // "ROOT",    // CPOSTAG
            // "ROOT",    // POSTAG
            // // "_",       // FEATS
            // "-1",      // HEAD
            // "ROOT"     // DEPREL
            // // "_",       // PHEAD
            // // "_"        // PDEPREL
            0,
            0,
            0,
            0,
            0
    );
    return token;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.getForm();
    return os;
}

Token::Token(const int id, const int form, const int postag, const int head, const int deprel)
        : id(id), form(form), postag(postag), head(head), deprel(deprel) {}

Token::Token(const string& id, const string& form, const string& postag, const string& head, const string& deprel)
        : Token(
                registerAttribute(Token::Atttribute::ID,     id    ),
                registerAttribute(Token::Atttribute::FORM,   form  ),
                registerAttribute(Token::Atttribute::POSTAG, postag),
                registerAttribute(Token::Atttribute::HEAD,   head  ),
                registerAttribute(Token::Atttribute::DEPREL, deprel)
        ) {}

int Token::registerAttribute(const Token::Atttribute name, const string& value) {
    return 0;
}

std::string Token::getAttribute(const int index) {
    return "test";
}

}
