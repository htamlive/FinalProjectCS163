#include "Trie.h"

Trie::Trie() {
	this->root = new TrieNode();
	(this->root) -> id = 0;
	this->size = 0;
}

void Trie::addWord(const string& word, const pair<int, int>& occurrence) {
	TrieNode* currentNode = this->root;
	int d;
	for (const char& c : word) {
		if (c < 'a' || c > 'z') {
			if (c != '.' && c != 39 && c != '-') {
				break;
			}
		}
		d = Trie::getID(c);
		if ((currentNode->children)[d] == nullptr) {
			(currentNode->children)[d] = new TrieNode;
			(currentNode->children)[d]->id = ++(this->size);
		}
		currentNode = (currentNode->children)[d];
	}
	(currentNode->occurences).push_back(occurrence);
};

void Trie::clearTrie() {
	(this->currentSearchString).clear();
	clearTrieNode(this->root);
	this->root = new TrieNode();
	(this->root)->id = 0;
	this->size = 0;
};

Trie::~Trie() {
	clearTrieNode(this->root);
};

void Trie::getListOfWords(TrieNode *node, string& current, int& remain, vector<string>& result) {
	if (node == nullptr || remain <= 0)
		return;
	if (!(node->occurences).empty()) {
		result.push_back(current);
		--remain;
	}
	if (remain <= 0)
		return;
	for (int i = 0; i < TrieNode::SIZE; ++i) {
		if ((node->children)[i] == nullptr)
			continue;
		current += (char)(i + Trie::offset);
		this->getListOfWords((node -> children)[i], current, remain, result);
		current.pop_back();
		if (remain <= 0)
			return;
	}
};

vector<string> Trie::getListOfWords(string prefix, int maximum) {
	vector<string> result;
	TrieNode* node = this->root;
	for (const char& c : prefix) {
		node = (node->children)[Trie::getID(c)];
		if (node == nullptr) {
			std::cerr << "No fucking way ...\n";
			return result;
		}
	}
	this->getListOfWords(node, prefix, maximum, result);
	return result;
};

int Trie::findIDofWord(const string& word) {
	/*
		return -1 if the trie does not contain word
	*/
	TrieNode* node = this->root;
	for (const char& c : word) {
		node = (node->children)[Trie::getID(c)];
		if (node == nullptr)
			return -1;
	}
	return node->id;
};