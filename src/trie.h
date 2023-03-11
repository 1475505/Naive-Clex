//
// Created by ll on 2023/3/11.
//

#ifndef CLEX_TRIE_H
#define CLEX_TRIE_H


#include <unordered_map>
#include <string>

using namespace std;

class Trie {
private:
    struct TrieNode {
        bool is_end = false;
        unordered_map<char, TrieNode*> children;
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->is_end = true;
    }

    bool search(const string& word) const {
        const TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) {
                return false;
            }
            node = node->children.at(c);
        }
        return node->is_end;
    }

    bool starts_with(const string& prefix) const {
        const TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children.count(c)) {
                return false;
            }
            node = node->children.at(c);
        }
        return true;
    }

    ~Trie() {
        delete_trie(root);
    }

private:
    void delete_trie(TrieNode* node) {
        if (!node) {
            return;
        }
        for (auto& child : node->children) {
            delete_trie(child.second);
        }
        delete node;
    }
};

#endif //CLEX_TRIE_H
