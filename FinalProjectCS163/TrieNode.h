#pragma once

#include <vector>
#include <string>

using namespace std;

class TrieNode {
public:
	const static int SIZE = 100;
	TrieNode* children[SIZE];

	int id;
	vector<pair<int, int> > occurences;
	//occurenece : {id of line, id of word (in the line)}

	TrieNode();
	TrieNode*& getChildNode(const int c);
};

void clearTrieNode(TrieNode*& node);