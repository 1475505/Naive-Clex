#include "token.h"
#include <iostream>

using namespace std;

namespace Lex {
    const std::vector<std::string> token_type = {
            "Identifier", "Keyword", "Integer", "Float", "Char", "String", "Punctuation", "Preprocess",
            "ErrorLex"};

    const std::vector<std::string> keywords = {
            "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
            "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };

    const std::vector<std::string> punctuators = {"", "[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*",
                                                  "+", "-",
                                                  "~", "!", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^",
                                                  "|",
                                                  "&&", "||", "?", ":", ";", "...", "=", "*=", "/=", "%=", "+=", "-=",
                                                  "<<=",
                                                  ">>=", "&=", "^=", "|=", ","};

    ostream &operator<<(ostream &os, const TokenPos &location) {
        os << location.file_name + ":" + to_string(location.row) + ":" + to_string(location.column);
        return os;
    }

    ostream &operator<<(ostream &os, const Token &token) {
        os << "[Pos] " << token.pos << '\n';
        os << "[Type] " << token_type[static_cast<size_t>(token.type)] << '\n';
        if (token.type == TokenType::ERRORLEX) {
            os << "[Value] ";
            if (token.errorLex == ErrorLex::NONE)
                os << "None";
            else if (token.errorLex == ErrorLex::UNCLOSED_COMMENT)
                os << "Unclosed Comment";
            else if (token.errorLex == ErrorLex::UNCLOSED_CHAR)
                os << "Unclosed Character" << token.literal;
            else if (token.errorLex == ErrorLex::UNCLOSED_STRING)
                os << "Unclosed String " << token.literal;
        } else if (token.type == TokenType::KEYWORD)
            os << "[Value] " << keywords[token.keyword];
        else if (token.type == TokenType::INTEGER || token.type == TokenType::FLOAT || token.type == TokenType::CHAR ||
                 token.type == TokenType::STRING || token.type == TokenType::IDENTIFIER)
            os << "[Value] " << token.literal;
        else if (token.type == TokenType::PUNCTUATION)
            os << "[Value] " << punctuators[static_cast<size_t>(token.punctuator)];
        os << '\n';
        return os;
    }
}
