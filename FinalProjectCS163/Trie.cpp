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
		if (!checkValidChar(c))
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
		if (!checkValidChar(c)) 
			return vector<pair<int, int> >();
		node = node->children[Trie::getID(tolower(c))];
		if (node == nullptr) {
			cerr << "The trie does not contain the given string.\n";
			return vector<pair<int, int> >();
		}
	}
	//cerr << (node->id) << '\n';
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
				previousOccurence = occurence.first;
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
				previousOccurence = occurence.first;
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
				previousOccurence = occurence.first;
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

bool Trie::removeOccurences(const string& word) {
	TrieNode* node = this->root;
	for (const char& c : word) {
		if (!checkValidChar(c))
			return false;
		node = node->children[Trie::getID(tolower(c))];
		if (node == nullptr)
			return false;
	}
	(node->occurences).clear();
	return true;
};

void Trie::buildSerialization(TrieNode* const node, stringstream& result) {
	if (node == nullptr)
		return;
	result << '{';
	for (int i = 0; i < TrieNode::SIZE; ++i) {
		if ((node->children)[i] == nullptr)
			continue;
		result << '(' << i << '|' << ((node -> children)[i] -> id);
		for (const auto& occurence : ((node->children)[i]->occurences))
			result << '|' << occurence.first << ',' << occurence.second;
		result << ')';
		buildSerialization((node->children)[i], result);
	}
	result << '}';
};

string Trie::serialize() const {
	stringstream result;
	buildSerialization(root, result);
	return result.str();
};

bool Trie::readNodeInformation(const string& serialization, int length, int &index, int& character, int& id, vector<pair<int, int> >& occurences) {
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

void Trie::buildSerialization(TrieNode* const node, ostream& result) {
	if (node == nullptr)
		return;
	result << '{';
	for (int i = 0; i < TrieNode::SIZE; ++i) {
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

void Trie::serialize(ostream& result) const {
	buildSerialization(root, result);
}

void Trie::deserialize(const string& serialization) {
	this->clearTrie();
	const int length = serialization.size();
	vector<pair<int, int> > occurences;
	vector<TrieNode*> stackOfNodes;
	TrieNode* node = this->root;
	int character, id;
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
			node = new TrieNode();
			node->id = id;
			node->occurences = occurences;
			++size;
			(stackOfNodes.back() -> children[character]) = node;
		}
	}
};

vector<int> Trie::filter(const DATASET& dataset, const vector<int>& id) const {
	int previousOccurence;
	vector<int> result;
	for (const int& i  : id) {
		previousOccurence = -1;
		for (const pair<int, int>& occurence : (this->getDefinitions(dataset.getData(i).first))) {
			if (previousOccurence == occurence.first)
				continue;
			result.push_back(occurence.first);
			previousOccurence = occurence.first;
		}
	}
	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	return result;
};