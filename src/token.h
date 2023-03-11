//
// Created by ll on 2023/3/11.
//

#ifndef CLEX_TOKEN_H
#define CLEX_TOKEN_H

#include <string>
#include <vector>

namespace Lex {
    /// 记号类型
    extern const std::vector<std::string> token_type;

    /// 记号类型
    enum TokenType {
        IDENTIFIER,
        KEYWORD,
        INTEGER,
        FLOAT,
        CHAR,
        STRING,
        PUNCTUATION,
        COMMENT,
        PREPROCESS,
        ERRORLEX
    };


    /// 关键字
    extern const std::vector<std::string> keywords;

    typedef int Keyword;

    /// 标点
    extern const std::vector<std::string> punctuators;

    /// 标点符号类型
    enum class Punctuator {
        NONE,
        LEFT_SQUARE, // [
        RIGHT_SQUARE, // ]
        LEFT_PAREN, // (
        RIGHT_PAREN, // )
        LEFT_BRACE, // {
        RIGHT_BRACE, // }
        DOT, // .
        ARROW, // ->
        PLUS_PLUS, // ++
        MINUS_MINUS, // --
        AMP, // &
        STAR, // *
        PLUS, // +
        MINUS, // -
        TILDE, // ~
        EXCLAIM, // !
        SLASH, // /
        PERCENT, // %
        LESS_LESS, // <<
        GREATER_GREATER, // >>
        LESS, // <
        GREATER, // >
        LESS_EQUAL, // <=
        GREATER_EQUAL, // >=
        EQUAL_EQUAL, // ==
        EXCLAIM_EQUAL, // !=
        CARET, // ^
        PIPE, // |
        AMP_AMP, // &&
        PIPE_PIPE, // ||
        QUESTION, // ?
        COLON, // :
        SEMICOLON, // ;
        DOTS, // ...
        EQUAL, // =
        STAR_EQUAL, // *=
        SLASH_EQUAL, // /=
        PERCENT_EQUAL, // %=
        PLUS_EQUAL, // +=
        MINUS_EQUAL, // -=
        LESS_LESS_EQUAL, // <<=
        GREATER_GREATER_EQUAL, // >>=
        AMP_EQUAL, // &=
        CARET_EQUAL, // ^=
        PIPE_EQUAL, // |=
        COMMA, // ,
    };

    /// 错误类型
    enum class ErrorLex {
        NONE,
        UNCLOSED_COMMENT,
        UNCLOSED_CHAR,
        UNCLOSED_STRING,
    };

    /// 记号位置类
    class TokenPos {
    public:
        TokenPos() = default;

        TokenPos(std::string file_name_, unsigned int row_, unsigned int column_) :
                file_name(std::move(file_name_)), row(row_), column(column_) {}

        friend std::ostream &operator<<(std::ostream &os, const TokenPos &location);

    private:
        std::string file_name;
        unsigned int row;
        unsigned int column;
    };

    /// 记号类
    class Token {
    public:
        Token() = default;

        Token(TokenType type_, TokenPos pos_, Keyword keyword_, Punctuator punctuator_, ErrorLex errorLex_,
              std::string literal_) :
                type(type_), pos(std::move(pos_)), keyword(keyword_), punctuator(punctuator_),
                errorLex(errorLex_), literal(std::move(literal_)) {}

        /// 构造关键字类型的记号
        Token(TokenType type_, TokenPos pos_, Keyword keyword_) :
                Token(type_, std::move(pos_), keyword_, Punctuator::NONE, ErrorLex::NONE, "") {}

        /// 构造标点符号类型的记号
        Token(TokenType type_, TokenPos Pos_, Punctuator punctuator_) :
                Token(type_, std::move(Pos_), 0, punctuator_, ErrorLex::NONE, "") {}

        /// 构造字面值类型的记号
        Token(TokenType type_, TokenPos Pos_, std::string literal_) :
                Token(type_, std::move(Pos_), 0, Punctuator::NONE, ErrorLex::NONE, std::move(literal_)) {}

        /// 构造未知类型的记号
        Token(TokenType type_, TokenPos Pos_, ErrorLex errorLex_) :
                Token(type_, std::move(Pos_), 0, Punctuator::NONE, errorLex_, "") {}

        /// 构造带字面值的未知类型的记号
        Token(TokenType type_, TokenPos Pos_, ErrorLex errorLex_, std::string literal_) :
                Token(type_, std::move(Pos_), 0, Punctuator::NONE, errorLex_, std::move(literal_)) {}

        TokenType GetType() const { return type; }

        friend std::ostream &operator<<(std::ostream &os, const Token &token);

    private:
        TokenType type;
        TokenPos pos;
        Keyword keyword; // 如果不为0，表示关键字
        Punctuator punctuator; // 如果不为0，表示标点
        ErrorLex errorLex; // 如果不为0，表示错误
        std::string literal; // 字面值
    };
}

#endif //CLEX_TOKEN_H
