#include <cassert>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

#include "Trie.h"
#include "helper.h"

Trie::Trie() {
	this->root = new TrieNode();
	(this->root) -> id = 0;
	this->size = 0;
}

void Trie::addWord(const string &word, const pair<int, int>& occurrence) {
	TrieNode* currentNode = this->root;
	int d;
	for (const char& c : word) {
		if (!checkPrintable(c))
			return;
		d = Trie::getID(tolower(c));
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

void Trie::getListOfWords(TrieNode* const node, string& current, int& remain, vector<string>& result) {
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
		getListOfWords((node -> children)[i], current, remain, result);
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
		if (node == nullptr) 
			return result;
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

void Trie::addCharacter(const char c) {
	(this->currentSearchString) += c;
};

void Trie::popCharacter() {
	assert(!(this->currentSearchString).empty());
	(this->currentSearchString).pop_back();
};

string Trie::getCurrentString() const {
	return this->currentSearchString;
};

vector<pair<int, int> > Trie::getDefinitions(const string &s) const {
	TrieNode* node = this->root;
	for (const char &c : s) {
		if (!checkPrintable(c)) 
			return vector<pair<int, int> >();
		node = node->children[Trie::getID(tolower(c))];
		if (node == nullptr) {
			cout << "Nullshit\n";
			return vector<pair<int, int> >();
		}
	}
	cout << node->id << '\n';
	return node->occurences;
};

bool Trie::containsWord(const string& s) const {
	TrieNode* node = this->root;
	for (const char& c : s) {
		node = node->children[Trie::getID(c)];
		if (node == nullptr)
			return false;
	}
	return !(node->occurences).empty();
};

void Trie::getIDofAllWords(TrieNode* const node, vector<int>& id) {
	if (node == nullptr)
		return;
	if (!(node->occurences).empty())
		id.push_back(node -> id);
	for (int i = 0; i < TrieNode::SIZE; ++i)
		getIDofAllWords((node -> children)[i], id);
};

vector<int> Trie::getIDofRandomWords(const int numberOfWords) const {
	vector<int> result;
	getIDofAllWords(this->root, result);
	srand(time(NULL));
	random_shuffle(result.begin(), result.end());
	result.resize(numberOfWords);
	return result;
};

vector<pair<int, int> > Trie::getKey(const DATASET &dataset, const string& definition) const {
	//Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<string> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const string& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStrings(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

vector<int> Trie::getKeys(const DATASET& dataset, const string& definition, const int maximum) const {
	// Return the (sorted) list of occurences (the line id) of words whose definitions contain set of words from input definition
	// The occurence in each node should be in sorted order (before calling this function)
	const vector<string> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const string& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStrings(splitString(toLowerString(definitionOfKey)), words)) {
				result.push_back(occurence.first);
				--remain;
			}
		}
	}
	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	if (result.size() >= maximum)
		result.resize(maximum);
	return result;
};

vector<pair<int, int> > Trie::getKeySubsequence(const DATASET& dataset, const string& definition) const {
	// Second type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<string> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const string& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubsequence(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

vector<pair<int, int> > Trie::getKeySubarray(const DATASET& dataset, const string& definition) const {
	// Third type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<string> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const string& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubarray(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

int Trie::getID(const char c) {
	return c - Trie::offset;
}

int Trie::getSize() const {
	return this->size;
}

bool Trie::empty() const {
	return (this->root) == nullptr;
}

vector<int> Trie::getKeysSubsequence(const DATASET& dataset, const string& definition, const int maximum) const {
	// Second type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<string> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const string& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubsequence(splitString(toLowerString(definitionOfKey)), words)) {
				result.push_back(occurence.first);
				--remain;
			}
		}
	}
	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	if (result.size() >= maximum)
		result.resize(maximum);
	return result;
};

vector<int> Trie::getKeysSubarray(const DATASET& dataset, const string& definition, const int maximum) const {
	// Third type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<string> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const string& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<string, string> data = dataset.getData(occurence.first);
			const string& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubarray(splitString(toLowerString(definitionOfKey)), words)) {
				result.push_back(occurence.first);
				--remain;
			}
		}
	}
	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	if (result.size() >= maximum)
		result.resize(maximum);
	return result;
};