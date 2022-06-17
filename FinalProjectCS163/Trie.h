#pragma once

#include <string>

using namespace std;

#include "TrieNode.h"

class Trie {
private:

	const static int offset = 32;

	string currentSearchString;
	TrieNode* root;
	int size;

	static int getID(const char c) {
		return c - offset;
	}

	void getListOfWords(TrieNode* node, string& current, int& remain, vector<string>& result);

public:
	
	Trie();
	~Trie();

	void addWord(const string &word, const string &defition);
	void addWord(const string& word, const pair<int, int>& occurence);
	vector<string> getListOfWords(string prefix, int maximum);
	int findIDofWord(const string& word);
	void clearTrie();

};