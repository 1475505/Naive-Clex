//
// Created by ll on 2023/3/11.
//

#ifndef CLEX_LEXER_H
#define CLEX_LEXER_H

#include "reader.h"
#include "trie.h"

namespace Lex {
    class Lexer {
    public:
        Lexer(std::string file_name_, Trie dict_) : reader(std::move(file_name_)), dict(dict_), lexeme_count() {}

        Token GetNextToken();

        friend std::ostream &operator<<(std::ostream &os, const Lexer &lexer);

    private:
        Reader reader;
        Trie dict;
        std::string buffer;
        TokenPos token_pos;
        unsigned int lexeme_count[10]; ///< 各种记号的数量

        Token GetTokenHandler();

        void LineCommentHandler();

        bool BlockCommentHandler();

        Token PreprocessingHandler();

        Token NumberTokenHandler();

        Token CharTokenHandler();

        Token StringTokenHandler();

        Token IdentifierTokenHandler();

        static bool isNumberCharacter(char c);

        Token DecimalIntegerHandler(std::string::iterator iter);

        Token DecimalFloatHandler(std::string::iterator iter);

        static bool isIdentifierCharacter(char c);
    };
}
#endif //CLEX_LEXER_H
