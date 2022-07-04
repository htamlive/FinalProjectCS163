#include "TrieNode.h"

TrieNode::TrieNode() {
	this->id = -1;
	for (int i = 0; i < SIZE; ++i)
		(this->children)[i] = nullptr;
}

void clearTrieNode(TrieNode* &node) {
	if (node == nullptr)
		return;
	for (int i = 0; i < TrieNode::SIZE; ++i)
		clearTrieNode((node -> children)[i]);
	delete node;
	node = nullptr;
}

TrieNode*& TrieNode::getChildNode(const int c) {
	return (this->children)[c];
};