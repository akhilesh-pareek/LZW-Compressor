#ifndef TRIE_H      // macro gaurd
#define TRIE_H
#include "settings.h"

// Definition of Trie Node
class TrieNode {
    int index;
    unsigned iter;
    TrieNode *child[MAXCHARS];
public:
    TrieNode(int index, unsigned iter);
    int getIndex();
    unsigned getIter();
    TrieNode *getChild(unsigned char ch);
    void setIter(unsigned iter);
    void setIndex(int index);
    void newChild(unsigned char ch, int index, unsigned iter);
    void updateChild(unsigned char ch, int index, unsigned iter);
};

// Definition of Trie
class Trie {
    // a trie is represented by pointer to its root
    TrieNode *root;
public:
    // constructor to initialize the Trie
    Trie();

    // accessor function for root
    TrieNode *getRoot();

    // add symbols from 0...MAXCHARS-1 in the Trie
    void initTrie(unsigned iter);

    void updateRootChildIterValue(unsigned iter);

    // empty the Trie i.e remove all symbols from the Trie
    void emptyTrie();
    void emptyTrieUtil(TrieNode *root);

};

#endif      // end of macro gaurd