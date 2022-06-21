#include <algorithm>

using namespace std;

#include "helper.h"
#include "Trie.h"

vector<string> splitString(const string& s) {
	//Split string by space
	vector<string> result;
	string t;
	for (const char& c : s) {
		if (c == ' ') {
			result.push_back(t);
			t.clear();
		} else
			t += c;
	}
	result.push_back(t);
	return result;
};

bool checkContainStrings(const vector<string> &s, const vector<string> &t) {
	//The characters of elements of s and t should be printable
	Trie trie;
	for (const string& e : s)
		trie.addWord(e, {10, 10});
	for (const string& e : t)
		if (!trie.containsWord(e))
			return false;
	return true;
};

bool checkPrintable(const char c) {
	return 32 <= c && c <= 127;
};

bool checkContainStringsAsSubsequence(const vector<string>& s, const vector<string>& t) {
	const int n = s.size(), m = t.size();
	for (int i = 0, j = 0; i < n && j < m; ++i) {
		if (s[i] == t[j]) {
			++j;
			if (j >= m)
				return true;
		}
	}
	return false;
};