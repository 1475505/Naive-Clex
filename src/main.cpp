/*+++++++++++++++++++++++++++++++++
C-Lex
@Date: 2023-3-11
@Ver: dev
+++++++++++++++++++++++++++++++++*/
//#define LOCAL
#include "trie.h"
#include "token.h"
#include <iostream>
using namespace std;
#define INF 0x3f3f3f3f
#define EPS 1e-6
const int MAX_N = 10000 + 5;

int main(int argc, char *argv[]) {
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    auto trie = Trie();
    for (const string& s : Lex::keywords) {
        trie.insert(s);
    }

    system("pause");
    return 0;
}
