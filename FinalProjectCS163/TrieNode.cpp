#include "TrieNode.h"

TrieNode::TrieNode() {
	this->id = -1;
	for (int i = 0; i < SIZE; ++i)
		(this->children)[i] = nullptr;
}