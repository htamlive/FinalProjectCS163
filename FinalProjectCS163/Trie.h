#pragma once

#include <string>
#include <iostream>

using namespace std;

#include "TrieNode.h"
#include "DATASET.h"

class Trie {
private:

	const static int offset = 32;

	string currentSearchString;
	TrieNode* root;
	int size;

	static int getID(const char c);
	static void getListOfWords(TrieNode* const node, string& current, int& remain, vector<string>& result);
	static void getIDofAllWords(TrieNode* const node, vector<int> &id);

public:
	
	Trie();
	~Trie();

	void addWord(const string& word, const pair<int, int>& occurence); 

	vector<pair<int, int> > getDefinitions(const string& s) const;

	vector<string> getListOfWords(string prefix, int maximum);

	vector<int> getIDofRandomWords(const int numberOfWords) const;

	vector<pair<int, int> > getKey(const DATASET& dataset, const string &definition) const;
	vector<pair<int, int> > getKeySubsequence(const DATASET& dataset, const string& definition) const;
	vector<pair<int, int> > getKeySubarray(const DATASET& dataset, const string& definition) const;
	vector<int> getKeys(const DATASET& dataset, const string& definition, const int maximum) const;
	vector<int> getKeysSubsequence(const DATASET& dataset, const string& definition, const int maximum) const;
	vector<int> getKeysSubarray(const DATASET& dataset, const string& definition, const int maximum) const;

	bool containsWord(const string& s) const;

	string getCurrentString() const;

	int findIDofWord(const string& word);

	void addCharacter(const char c);

	void popCharacter();

	int getSize() const;

	bool empty() const;
	
	void clearTrie();

};