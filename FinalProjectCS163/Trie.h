#pragma once

#include <string>

using namespace std;

#include "TrieNode.h"

class Trie {
private:

	string currentSearchString;
	TrieNode* root;
	int size;

	static int getID(const char c) {
		const static int offset = 32;
		return c - offset;
	}

public:
	
	Trie();

	void addWord(const string &word, const string &defition);

};