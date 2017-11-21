#include "settings.h"
#include "Trie.h"

TrieNode::TrieNode(int index, unsigned iter) : index(index), iter(iter) {
    for (int i=0; i<MAXCHARS; ++i)
        child[i] = NULL;
}

int TrieNode::getIndex() {
    return index;
}

unsigned TrieNode::getIter() {
    return iter;
}

void TrieNode::setIter(unsigned iter) {
    this->iter = iter;
}

// since the values of char will be in [-128...127], so it is casted into unsigned char when passing to bring in range [0...255]
TrieNode *TrieNode::getChild(unsigned char ch) {
    // child of this TrieNode corresponding to character number ch
    return child[ch];   
}

// sets a new TrieNode as the ch'th child of given object
void TrieNode::newChild(unsigned char ch, int index, unsigned iter) {
    child[ch] = new TrieNode(index, iter);
}

void TrieNode::setIndex(int index) {
    this->index = index;
}

void TrieNode::updateChild(unsigned char ch, int index, unsigned iter) {
    child[ch]->index = index;
    child[ch]->iter = iter;
}

// definitions for memeber functions of Trie
Trie::Trie() {
    // initialize trie root to NULL
    root = NULL;        
}

TrieNode *Trie::getRoot() {
    return root;
}

void Trie::initTrie(unsigned iter) {
    root = new TrieNode(-1, iter);
    for (int i=0; i<MAXCHARS; ++i)
        root->newChild(i, i, iter);
}

// called when number of symbols == TABLE_SIZE, and iter is changed
// no need to update iter value for root
void Trie::updateRootChildIterValue(unsigned iter) {
    for (int i=0; i<MAXCHARS; ++i)
        root->getChild(i)->setIter(iter);
}

void Trie::emptyTrieUtil(TrieNode *root) {
    if (root != NULL) {
        for (int i=0; i<MAXCHARS; ++i)
            emptyTrieUtil(root->getChild(i));
        delete root;
    }
}

void Trie::emptyTrie() {
    emptyTrieUtil(root);
    root = NULL;    // since now the trie is empty
}