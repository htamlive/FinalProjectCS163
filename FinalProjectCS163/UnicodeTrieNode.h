#pragma once

#include <vector>
#include <string>

using namespace std;

class UnicodeTrieNode {
public:
	const static int SIZE = 150;
	UnicodeTrieNode* children[SIZE];

	int id;
	vector<pair<int, int> > occurences;
	//occurenece : {id of line, id of word (in the line)}

	UnicodeTrieNode();
};

void clearUnicodeTrieNode(UnicodeTrieNode*& node);