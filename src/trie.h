//
// Created by ll on 2023/3/11.
//

#ifndef CLEX_TRIE_H
#define CLEX_TRIE_H


#include <unordered_map>
#include <string>
#include "assert.h"

using namespace std;

class TrieNode {
public:
    std::unordered_map<char, TrieNode *> children;
    bool is_end_of_word;

    TrieNode() : is_end_of_word(false) {}
};

class Trie {
private:
    TrieNode *root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string &word) {
        TrieNode *current = root;
        for (const auto &ch: word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->is_end_of_word = true;
    }

    bool search(const std::string &word) {
        TrieNode *current = root;
        for (const auto &ch: word) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return current->is_end_of_word;
    }

    bool starts_with(const std::string &prefix) {
        TrieNode *current = root;
        for (const auto &ch: prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return true;
    }
};

#endif //CLEX_TRIE_H
