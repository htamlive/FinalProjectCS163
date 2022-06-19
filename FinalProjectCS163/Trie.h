#pragma once

#include <string>
#include <iostream>

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

	void addWord(const string& word, const pair<int, int>& occurence);
	vector<pair<int, int> > getDefinitions(const string& s) const;
	vector<string> getListOfWords(string prefix, int maximum);
	string getCurrentString() const;
	int findIDofWord(const string& word);
	void addCharacter(const char c);
	void popCharacter();
	void clearTrie();

};