//
// Created by ll on 2023/3/11.
//

#include "lexer.h"
#include <iostream>

using namespace std;

namespace Lex {
    Token Lexer::GetNextToken() {
        Token token = GetTokenHandler();
        ++lexeme_count[static_cast<size_t>(token.GetType())];
        return token;
    }

    Token Lexer::GetTokenHandler() {
        buffer.clear();
        if (reader.Eof())
            return {TokenType::END_OF_FILE, reader.GetLocation(), ""};
        char c;
        bool is_leading_blank;
        do {
            is_leading_blank = reader.GetisLeadingBlank();
            if (reader.Eof())
                return {TokenType::END_OF_FILE, reader.GetLocation(), ""};
            c = reader.GetChar();
        } while (isspace(c)); // 跳过空白字符

        token_pos = reader.GetLocation();
        switch (c) {
            case '[':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::LEFT_SQUARE};
            case ']':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::RIGHT_SQUARE};
            case '(':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::LEFT_PAREN};
            case ')':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::RIGHT_PAREN};
            case '{':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::LEFT_BRACE};
            case '}':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::RIGHT_BRACE};
            case '.':
                if (reader.GetCharN(1) == '.' && reader.GetCharN(2) == '.') {
                    reader.SkipCharN(2);
                    return {TokenType::PUNCTUATION, token_pos, Punctuator::DOTS};
                } else if (isdigit(reader.GetCharN(1))) {
                    buffer.push_back(c);
                    return NumberTokenHandler(); // 小数点后接数字
                } else
                    return {TokenType::PUNCTUATION, token_pos, Punctuator::DOT};
            case '-':
                switch (reader.GetCharN(1)) {
                    case '>':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::ARROW};
                    case '-':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::MINUS_MINUS};
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::MINUS_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::MINUS};
                }
            case '+':
                switch (reader.GetCharN(1)) {
                    case '+':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PLUS_PLUS};
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PLUS_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PLUS};
                }
            case '&':
                switch (reader.GetCharN(1)) {
                    case '&':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::AMP_AMP};
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::AMP_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::AMP};
                }
            case '*':
                switch (reader.GetCharN(1)) {
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::STAR_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::STAR};
                }
            case '~':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::TILDE};
            case '!':
                switch (reader.GetCharN(1)) {
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::EXCLAIM_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::EXCLAIM};
                }
            case '/':
                switch (reader.GetCharN(1)) {
                    case '/':
                        reader.SkipCharN(1);
                        LineCommentHandler(); // 行内注释
                        return GetTokenHandler(); // 跳过注释后获取下一个记号
                    case '*':
                        reader.SkipCharN(1);
                        if (!BlockCommentHandler()) // 块注释
                            return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_COMMENT};
                        return GetTokenHandler(); // 跳过注释后获取下一个记号
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::SLASH_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::SLASH};
                }
            case '%':
                switch (reader.GetCharN(1)) {
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PERCENT_EQUAL};
                    case '>': // }
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::RIGHT_BRACE};
                    case ':': // #
                        if (reader.GetCharN(2) == '%' && reader.GetCharN(3) == ':') // ##
                        {
                            reader.SkipCharN(3);
                            return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO};
                        } else if (is_leading_blank) {
                            reader.SkipCharN(1); // 在行首，是合法预处理器
                            return PreprocessingHandler();
                        } else {
                            reader.SkipCharN(1);
                            return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO};
                        }
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PERCENT};
                }
            case '<':
                switch (reader.GetCharN(1)) {
                    case '<':
                        if (reader.GetCharN(2) == '=') {
                            reader.SkipCharN(2);
                            return {TokenType::PUNCTUATION, token_pos, Punctuator::LESS_LESS_EQUAL};
                        } else {
                            reader.SkipCharN(1);
                            return {TokenType::PUNCTUATION, token_pos, Punctuator::LESS_LESS};
                        }
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::LESS_EQUAL};
                    case ':': // [
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::LEFT_SQUARE};
                    case '%': // {
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::LEFT_BRACE};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::LESS};
                }
            case '>':
                switch (reader.GetCharN(1)) {
                    case '>':
                        if (reader.GetCharN(2) == '=') {
                            reader.SkipCharN(2);
                            return {TokenType::PUNCTUATION, token_pos, Punctuator::GREATER_GREATER_EQUAL};
                        } else {
                            reader.SkipCharN(1);
                            return {TokenType::PUNCTUATION, token_pos, Punctuator::GREATER_GREATER};
                        }
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::GREATER_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::GREATER};
                }
            case '=':
                switch (reader.GetCharN(1)) {
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::EQUAL_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::EQUAL};
                }
            case '^':
                switch (reader.GetCharN(1)) {
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::CARET_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::CARET};
                }
            case '|':
                switch (reader.GetCharN(1)) {
                    case '|':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PIPE_PIPE};
                    case '=':
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PIPE_EQUAL};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::PIPE};
                }
            case '?':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::QUESTION};
            case ':':
                switch (reader.GetCharN(1)) {
                    case '>': // ]
                        reader.SkipCharN(1);
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::RIGHT_SQUARE};
                    default:
                        return {TokenType::PUNCTUATION, token_pos, Punctuator::COLON};
                }
            case ';':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::SEMICOLON};
            case ',':
                return {TokenType::PUNCTUATION, token_pos, Punctuator::COMMA};
            case '#':
                switch (reader.GetCharN(1)) {
                    case '#':
                        reader.SkipCharN(1);
                        return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO};
                    default:
                        if (is_leading_blank) // 合法预处理
                            return PreprocessingHandler();
                        else
                            return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO};
                }
            case '\'':
                return CharTokenHandler();
            case '\"':
                return StringTokenHandler();
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                buffer.push_back(c);
                return NumberTokenHandler();
            case 'L':
                switch (reader.GetCharN(1)) {
                    case '\'':
                        reader.SkipCharN(1);
                        return CharTokenHandler();
                    case '\"':
                        reader.SkipCharN(1);
                        return StringTokenHandler();
                    default:
                        buffer.push_back(c);
                        return IdentifierTokenHandler();
                }
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '_':
                buffer.push_back(c);
                return IdentifierTokenHandler();
            default:
                buffer.push_back(c);
                return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO, buffer};
        }
    }

    void Lexer::LineCommentHandler() {
        while (!reader.Eof() && reader.GetChar() != '\n');
    }

    bool Lexer::BlockCommentHandler() {
        while (!reader.Eof()) {
            if (reader.GetChar() == '*' && reader.GetCharN(1) == '/') {
                reader.SkipCharN(1);
                return true;
            }
        }
        return false;
    }

    Token Lexer::PreprocessingHandler() {
        char c;
        while (!reader.Eof()) {
            c = reader.GetChar();
            if (c == '\n')break;
            if (c == '\\') {
                do {
                    if (reader.Eof())return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_CHAR};
                    c = reader.GetChar();
                } while (c != '\n' && isblank(c));
            }
        }
        return {TokenType::PREPROCESS, token_pos, ""};
    }

    Token Lexer::NumberTokenHandler() {
        while (!reader.Eof() && (isNumberCharacter(reader.GetCharN(1)) ||
                                 (reader.GetCharN(1) == '+' || reader.GetCharN(1) == '-'))) {
            buffer.push_back(reader.GetChar());
        }
        if (buffer[0]) {
            return DecimalIntegerHandler(buffer.begin());
        }
        return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO, buffer};
    }

    bool Lexer::isNumberCharacter(char c) {
        return isdigit(c) || c == '.';
    }

    Token Lexer::DecimalIntegerHandler(string::iterator iter) {
        while (iter != buffer.end() && isdigit(*iter))
            ++iter;
        if (iter == buffer.end()) {
            return {TokenType::INTEGER, token_pos, buffer};
        }
        if (*iter == '.') {
            return DecimalFloatHandler(iter);
        }
        return {TokenType::ERRORLEX, token_pos, ErrorLex::TODO, buffer};
    }


    Token Lexer::DecimalFloatHandler(std::string::iterator iter) {
        if (*iter == '.') {
            ++iter;
            while (isdigit(*iter)) ++iter;
        }
        return {TokenType::FLOAT, token_pos, buffer};
    }

    Token Lexer::CharTokenHandler() {
        char c;
        while (!reader.Eof()) {
            c = reader.GetChar();
            if (c == '\'')
                return {TokenType::CHAR, token_pos, buffer};
            if (c == '\n')
                return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_CHAR, buffer};
            if (c == '\\') {
                std::string tempString{c};
                do {
                    if (reader.Eof())return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_CHAR};
                    c = reader.GetChar();
                    tempString.push_back(c);
                } while (c != '\n' && isblank(c));
                if (c == '\n')continue;
                buffer += tempString; // 如果反斜杠后跟的不是空格和换行，则反斜杠及其后的部分要加入缓冲区
            } else
                buffer.push_back(c);
        }
        return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_CHAR, buffer};
    }

    Token Lexer::StringTokenHandler() {
        char c;
        while (!reader.Eof()) {
            c = reader.GetChar();
            if (c == '\"')
                return {TokenType::STRING, token_pos, buffer};
            if (c == '\n')
                return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_STRING, buffer};
            if (c == '\\') {
                std::string tempString{c};
                do {
                    if (reader.Eof())return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_CHAR};
                    c = reader.GetChar();
                    tempString.push_back(c);
                } while (c != '\n' && isblank(c));
                if (c == '\n')continue;
                buffer += tempString; // 如果反斜杠后跟的不是空格和换行，则反斜杠及其后的部分要加入缓冲区
            } else
                buffer.push_back(c);
        }
        return {TokenType::ERRORLEX, token_pos, ErrorLex::UNCLOSED_STRING, buffer};
    }

    bool Lexer::isIdentifierCharacter(char c) {
        return c == '_' || isalnum(c);
    }

    Token Lexer::IdentifierTokenHandler() {
        while (!reader.Eof() && isIdentifierCharacter(reader.GetCharN(1)))
            buffer.push_back(reader.GetChar());
        auto keyword = dict.search(buffer);
        if (keyword)
            return {TokenType::KEYWORD, token_pos, buffer};
        return {TokenType::IDENTIFIER, token_pos, buffer};
    }

    std::ostream &operator<<(ostream &os, const Lexer &lexer) {
        os << "File: " << lexer.reader.GetFileName() << endl;
        os << "Rows: " << lexer.reader.GetRow() << endl;
        os << "Character: " << lexer.reader.GetCountChar() << endl;
        for (int i = 0; i < token_type.size(); ++i)
            os << "(" << token_type[i] << ") " << lexer.lexeme_count[i] << endl;
        return os;
    }
}