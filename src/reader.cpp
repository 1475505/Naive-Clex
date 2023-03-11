//
// Created by ll on 2023/3/11.
//

#include "reader.h"

namespace Lex {
    bool Reader::Eof() {
        return source_file.eof() && buf.empty();
    }

    char Reader::GetChar() {
        if (Eof())
            return 0;
        if (buf.empty())
            Read();

        char ch = buf.front();
        buf.pop_front();

        if (ch != '\n') {
            ++column;
            is_leading_blank &= (ch == ' ');
        } else {
            ++row;
            column = 0;
            is_leading_blank = true;

        }
        char_cnt++;
        return ch;
    }

    char Reader::GetCharN(size_t offset) {
        while (!source_file.eof() && buf.size() < offset)
            Read();
        return buf.at(offset - 1);
    }

    void Reader::SkipCharN(int num) {
        while (num--)
            GetChar();
    }

    void Reader::Read() {
        if (source_file.eof())return;
        char c = source_file.get();
        buf.push_back(c);
        source_file.peek();
    }
}
