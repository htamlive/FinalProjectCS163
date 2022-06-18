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

	TrieNode();
};

void clearTrieNode(TrieNode*& node);