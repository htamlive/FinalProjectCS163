#include "Trie.h"

Trie::Trie() {
	this->root = new TrieNode();
	(this->root) -> id = 0;
	this->size = 0;
}

void Trie::addWord(const string& word, const string& defition) {
	TrieNode* currentNode = this->root;
	int d;
	for (const char &c : word) {
		d = Trie::getID(c);
		if ((currentNode->children)[d] == nullptr) {
			(currentNode->children)[d] = new TrieNode;
			(currentNode->children)[d]->id = ++(this->size);
		}
		currentNode = (currentNode->children)[d];
	}
	(currentNode->defitions).push_back(defition);
};