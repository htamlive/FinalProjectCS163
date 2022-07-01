#include <cassert>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

#include "UnicodeTrie.h"
#include "helper.h"

UnicodeTrie::UnicodeTrie() {
	this->root = new UnicodeTrieNode();
	(this->root)->id = 0;
	this->size = 0;
}

void UnicodeTrie::addWord(const tgui::String& word, const pair<int, int>& occurrence) {
	UnicodeTrieNode* currentNode = this->root;
	int d;
	for (const char& c : word) {
		if (!checkPrintable(c))
			return;
		d = UnicodeTrie::getID(tolower(c));
		if ((currentNode->children)[d] == nullptr) {
			(currentNode->children)[d] = new UnicodeTrieNode;
			(currentNode->children)[d]->id = ++(this->size);
		}
		currentNode = (currentNode->children)[d];
	}
	(currentNode->occurences).push_back(occurrence);
};

void UnicodeTrie::clearTrie() {
	(this->currentSearchString).clear();
	clearUnicodeTrieNode(this->root);
	this->root = new UnicodeTrieNode();
	(this->root)->id = 0;
	this->size = 0;
};

UnicodeTrie::~UnicodeTrie() {
	clearUnicodeTrieNode(this->root);
};

int UnicodeTrie::getCharacter(const int id) {
	if (id < UnicodeTrie::offset2)
		return id + offset1;
	return SOURCE_CHARACTERS[id - UnicodeTrie::offset2];
};

void UnicodeTrie::getListOfWords(UnicodeTrieNode* const node, tgui::String& current, int& remain, vector<tgui::String>& result) {
	if (node == nullptr || remain <= 0)
		return;
	if (!(node->occurences).empty()) {
		result.push_back(current);
		--remain;
	}
	if (remain <= 0)
		return;
	for (int i = 0; i < UnicodeTrieNode::SIZE; ++i) {
		if ((node->children)[i] == nullptr)
			continue;
		current += tgui::String(getCharacter(i));
		getListOfWords((node->children)[i], current, remain, result);
		current.pop_back();
		if (remain <= 0)
			return;
	}
};

vector<tgui::String> UnicodeTrie::getListOfWords(tgui::String prefix, int maximum) {
	vector<tgui::String> result;
	UnicodeTrieNode* node = this->root;
	for (const char& c : prefix) {
		node = (node->children)[UnicodeTrie::getID(c)];
		if (node == nullptr)
			return result;
	}
	this->getListOfWords(node, prefix, maximum, result);
	return result;
};

int UnicodeTrie::findIDofWord(const tgui::String& word) {
	/*
		return -1 if the trie does not contain word
	*/
	UnicodeTrieNode* node = this->root;
	for (const char& c : word) {
		node = (node->children)[UnicodeTrie::getID(c)];
		if (node == nullptr)
			return -1;
	}
	return node->id;
};

void UnicodeTrie::addCharacter(const char c) {
	(this->currentSearchString) += c;
};

void UnicodeTrie::popCharacter() {
	assert(!(this->currentSearchString).empty());
	(this->currentSearchString).pop_back();
};

tgui::String UnicodeTrie::getCurrentString() const {
	return this->currentSearchString;
};

vector<pair<int, int> > UnicodeTrie::getDefinitions(const tgui::String& s) const {
	UnicodeTrieNode* node = this->root;
	for (const char& c : s) {
		if (!checkPrintable(c))
			return vector<pair<int, int> >();
		node = node->children[UnicodeTrie::getID(tolower(c))];
		if (node == nullptr) {
			cerr << "The trie does not contain tgui::String.\n";
			return vector<pair<int, int> >();
		}
	}
	return node->occurences;
};

bool UnicodeTrie::containsWord(const tgui::String& s) const {
	UnicodeTrieNode* node = this->root;
	for (const char& c : s) {
		node = node->children[UnicodeTrie::getID(c)];
		if (node == nullptr)
			return false;
	}
	return !(node->occurences).empty();
};

void UnicodeTrie::getIDofAllWords(UnicodeTrieNode* const node, vector<int>& id) {
	if (node == nullptr)
		return;
	if (!(node->occurences).empty())
		id.push_back(node->id);
	for (int i = 0; i < UnicodeTrieNode::SIZE; ++i)
		getIDofAllWords((node->children)[i], id);
};

vector<int> UnicodeTrie::getIDofRandomWords(const int numberOfWords) const {
	vector<int> result;
	getIDofAllWords(this->root, result);
	srand(time(NULL));
	random_shuffle(result.begin(), result.end());
	result.resize(numberOfWords);
	return result;
};

vector<pair<int, int> > UnicodeTrie::getKey(const DATASET& dataset, const tgui::String& definition) const {
	//Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<tgui::String> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const tgui::String& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
			if (checkContainStrings(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

vector<int> UnicodeTrie::getKeys(const DATASET& dataset, const tgui::String& definition, const int maximum) const {
	// Return the (sorted) list of occurences (the line id) of words whose definitions contain set of words from input definition
	// The occurence in each node should be in sorted order (before calling this function)
	const vector<tgui::String> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const tgui::String& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
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

vector<pair<int, int> > UnicodeTrie::getKeySubsequence(const DATASET& dataset, const tgui::String& definition) const {
	// Second type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<tgui::String> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const tgui::String& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubsequence(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

vector<pair<int, int> > UnicodeTrie::getKeySubarray(const DATASET& dataset, const tgui::String& definition) const {
	// Third type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<tgui::String> words = splitString(definition);
	vector<pair<int, int> > result;
	for (const tgui::String& word : words) {
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
			if (checkContainStringsAsSubarray(splitString(toLowerString(definitionOfKey)), words))
				result.push_back(occurence);
		}
	}
	sort(result.begin(), result.end());
	return result;
};

int UnicodeTrie::getID(const char c) {
	if (32 <= c && c <= 127)
		return c - UnicodeTrie::offset1;
	return UnicodeTrie::offset2 + (lower_bound(SOURCE_CHARACTERS.begin(), SOURCE_CHARACTERS.end(), c) - SOURCE_CHARACTERS.begin());
}

int UnicodeTrie::getSize() const {
	return this->size;
}

bool UnicodeTrie::empty() const {
	return (this->root) == nullptr;
}

vector<int> UnicodeTrie::getKeysSubsequence(const DATASET& dataset, const tgui::String& definition, const int maximum) const {
	// Second type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<tgui::String> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const tgui::String& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
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

vector<int> UnicodeTrie::getKeysSubarray(const DATASET& dataset, const tgui::String& definition, const int maximum) const {
	// Third type searching
	// Return the (sorted) list of occurences of words whose definitions contain set of words from input definition
	const vector<tgui::String> words = splitString(definition);
	int previousOccurence, remain;
	vector<int> result;
	for (const tgui::String& word : words) {
		previousOccurence = -1;
		remain = maximum;
		for (const pair<int, int>& occurence : (this->getDefinitions(word))) {
			if (remain <= 0)
				break;
			if (previousOccurence == occurence.first)
				continue;
			const pair<tgui::String, tgui::String> data = dataset.getData(occurence.first);
			const tgui::String& key = data.first, & definitionOfKey = data.second;
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

bool UnicodeTrie::removeOccurences(const tgui::String& word) {
	UnicodeTrieNode* node = this->root;
	for (const char& c : word) {
		if (!checkPrintable(c))
			return false;
		node = node->children[UnicodeTrie::getID(tolower(c))];
		if (node == nullptr)
			return false;
	}
	(node->occurences).clear();
	return true;
};

void UnicodeTrie::buildSerialization(UnicodeTrieNode* const node, stringstream& result) {
	if (node == nullptr)
		return;
	result << '{';
	for (int i = 0; i < UnicodeTrieNode::SIZE; ++i) {
		if ((node->children)[i] == nullptr)
			continue;
		result << '(' << i << '|' << ((node->children)[i]->id);
		for (const auto& occurence : ((node->children)[i]->occurences))
			result << '|' << occurence.first << ',' << occurence.second;
		result << ')';
		buildSerialization((node->children)[i], result);
	}
	result << '}';
};

std::string UnicodeTrie::serialize() const {
	stringstream result;
	buildSerialization(root, result);
	return result.str();
};

bool UnicodeTrie::readNodeInformation(const std::string& serialization, int length, int& index, int& character, int& id, vector<pair<int, int> >& occurences) {
	if (index >= length || serialization[index] != '(')
		return false;
	readInteger(serialization, ++index, character);
	if (index >= length || serialization[index] != '|')
		return false;
	readInteger(serialization, ++index, id);
	occurences.clear();
	while (index < length) {
		if (serialization[index] == ')') {
			++index;
			return true;
		}
		if (serialization[index] != '|')
			return false;
		++index;
		occurences.emplace_back(0, 0);
		readInteger(serialization, index, occurences.back().first);
		if (index >= length || serialization[index] != ',')
			return false;
		readInteger(serialization, ++index, occurences.back().second);
	}
	return false;
};

void UnicodeTrie::deserialize(const std::string& serialization) {
	const int length = serialization.size();
	vector<pair<int, int> > occurences;
	vector<UnicodeTrieNode*> stackOfNodes;
	UnicodeTrieNode* node;
	int character, id;
	this->clearTrie();
	node = this->root;
	for (int i = 0; i < length;) {
		if (serialization[i] == '{') {
			stackOfNodes.push_back(node);
			++i;
			continue;
		}
		if (serialization[i] == '}') {
			stackOfNodes.pop_back();
			++i;
			continue;
		}
		if (readNodeInformation(serialization, length, i, character, id, occurences)) {
			node = new UnicodeTrieNode();
			node->id = id;
			node->occurences = occurences;
			++size;
			(stackOfNodes.back()->children[character]) = node;
		}
	}
};