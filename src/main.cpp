/*+++++++++++++++++++++++++++++++++
C-Lex
@Date: 2023-3-11
@Ver: dev
+++++++++++++++++++++++++++++++++*/
//#define LOCAL
#include "trie.h"
#include "token.h"
#include <iostream>
#include "lexer.h"

using namespace std;
#define INF 0x3f3f3f3f
#define EPS 1e-6
const int MAX_N = 10000 + 5;

int main(int argc, char *argv[]) {
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    printf("  ___  __    ____  _  _ \n");
    printf(" / __)(  )  (  __)( \\/ )\n");
    printf("( (__ / (_/\\ ) _)  )  ( \n");
    printf(" \\___)\\____/(____)(_\\/_) \n");
    printf("    C-Lex by liuliang \n");
    auto trie = Trie();
    for (const string &s: Lex::keywords) {
        trie.insert(s);
    }

    char *file_name = "../codes/foo.c";;
    if (argc > 1) {
        file_name = argv[1];
    } else {
        printf("usage: ./exe [filepath], now use ../codes/foo.c as default. \n");
    }
    auto lexer = Lex::Lexer(file_name, trie);
    cout << "-------------Token-----------" << endl;
    Lex::Token token = lexer.GetNextToken();
    while (token.GetType() != Lex::TokenType::END_OF_FILE) {
        cout << token << endl;
        token = lexer.GetNextToken();
    }
    cout << "------------Analysis---------" << endl;
    cout << lexer << endl;

    system("pause");
    return 0;
}
