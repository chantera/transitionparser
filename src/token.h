//
// Created by Hiroki Teranishi on 4/7/17.
//

#ifndef COORDPARSER_TOKEN_H
#define COORDPARSER_TOKEN_H

#include <string>
#include <vector>

using std::string;

namespace coordparser {

class Token {

public:
    const int id;
    const int form;
    // const int lemma;
    // const int cpostag;
    const int postag;
    // const int feats;
    const int head;
    const int deprel;
    // const int phead;
    // const int pdeprel;

    enum Atttribute {
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

    Token(const std::vector<string>& attributes);

    Token(const string& id, const string& form, const string& lemma, const string& cpostag, const string& postag,
          const string& feats, const string& head, const string& deprel, const string& phead, const string& pdeprel);

    std::string getForm() const;

    std::string getPostag() const;

    std::string getDeprel() const;

    static Token createRoot();

    friend std::ostream& operator<<(std::ostream& os, const Token& token);

private:
    Token(const int id, const int form, const int postag, const int head, const int deprel);

    Token(const string& id, const string& form, const string& postag, const string& head, const string& deprel);

    static int registerAttribute(const Atttribute name, const string& value);

    static std::string getAttribute(const int index);
};

}


#endif //COORDPARSER_TOKEN_H
