#pragma once

#include <string>
#include <iostream>

using namespace std;

#include "UnicodeTrieNode.h"
#include "DATASET.h"
#include "helper.h"

class UnicodeTrie {
private:

	const static int offset1 = 32, offset2 = 96/*127 - 32 + 1*/;

	tgui::String currentSearchString;
	UnicodeTrieNode* root;
	int size;

	static int getID(const char c);
	static int getCharacter(const int id);
	static void getListOfWords(UnicodeTrieNode* const node, tgui::String& current, int& remain, vector<tgui::String>& result);
	static void getIDofAllWords(UnicodeTrieNode* const node, vector<int>& id);
	static void buildSerialization(UnicodeTrieNode* const node, stringstream& result);
	static bool readNodeInformation(const std::string & serialization, int length, int& index, int& character, int& id, vector<pair<int, int> >& occurences);

public:

	UnicodeTrie();
	~UnicodeTrie();

	void addWord(const tgui::String& word, const pair<int, int>& occurence);

	vector<pair<int, int> > getDefinitions(const tgui::String& s) const;

	vector<tgui::String> getListOfWords(tgui::String prefix, int maximum);

	vector<int> getIDofRandomWords(const int numberOfWords) const;

	vector<pair<int, int> > getKey(const DATASET& dataset, const tgui::String& definition) const;

	vector<pair<int, int> > getKeySubsequence(const DATASET& dataset, const tgui::String& definition) const;

	vector<pair<int, int> > getKeySubarray(const DATASET& dataset, const tgui::String& definition) const;

	vector<int> getKeys(const DATASET& dataset, const tgui::String& definition, const int maximum) const;

	vector<int> getKeysSubsequence(const DATASET& dataset, const tgui::String& definition, const int maximum) const;

	vector<int> getKeysSubarray(const DATASET& dataset, const tgui::String& definition, const int maximum) const;

	bool containsWord(const tgui::String& s) const;

	tgui::String getCurrentString() const;

	int findIDofWord(const tgui::String& word);

	void addCharacter(const char c);

	void popCharacter();

	int getSize() const;

	bool empty() const;

	bool removeOccurences(const tgui::String& word);

	void clearTrie();

	std::string serialize() const;

	void deserialize(const std::string& serialization);

};