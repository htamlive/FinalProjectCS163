#include "UnicodeTrieNode.h"

UnicodeTrieNode::UnicodeTrieNode() {
	this->id = -1;
	for (int i = 0; i < SIZE; ++i)
		(this->children)[i] = nullptr;
}

void clearUnicodeTrieNode(UnicodeTrieNode*& node) {
	if (node == nullptr)
		return;
	for (int i = 0; i < UnicodeTrieNode::SIZE; ++i)
		clearUnicodeTrieNode((node->children)[i]);
	delete node;
	node = nullptr;
}

UnicodeTrieNode*& UnicodeTrieNode::getChildNode(const int c) {
	return (this->children)[c];
};