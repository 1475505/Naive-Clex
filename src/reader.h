#ifndef CLEX_SCANNER_H
#define CLEX_SCANNER_H

#include <fstream>
#include "token.h"
#include "assert.h"
#include <deque>

namespace Lex {
    class Reader {
    public:
        explicit Reader(std::string file_name_) :
                file_name(std::move(file_name_)), row(1), column(0), char_cnt(0), buf(), is_leading_blank(true) {
            source_file.open(file_name);
            assert(!source_file.fail());
        }

        bool Eof();

        // 文件流中的下一个字符
        char GetChar();

        // 从当前字符后第offset个字符
        char GetCharN(size_t offset);

        // 跳过字符
        void SkipCharN(int num);

        const std::string &GetFileName() const { return file_name; }

        unsigned int GetRow() const { return row; }

        unsigned int GetCountChar() const { return char_cnt; }

        bool GetisLeadingBlank() const { return is_leading_blank; }

        Lex::TokenPos GetLocation() const { return {file_name, row, column}; }

    private:
        std::string file_name;
        std::ifstream source_file; // 文件流
        unsigned int row;
        unsigned int column;
        unsigned int char_cnt; // 当前字符数
        std::deque<char> buf; // 缓冲区
        bool is_leading_blank; //是否是行首（空格部分）
        void Read();
    };
}

#endif
