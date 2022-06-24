#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

#include "helper.h"
#include "Trie.h"
#include "Hash.h"

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

int getRandomNumberFromList(const vector<int>& numbers) {
	srand(time(NULL));
	return numbers[rand() % numbers.size()];
};

bool checkContainStringsAsSubarray(const vector<string>& s, const vector<string>& t) {
	if (t.empty())
		return true;
	const int n = s.size(), m = t.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	if (n < m)
		return false;
	HashedString a(base, MOD), b(base, MOD);
	for (const string& e : t) {
		if (!b.empty())
			b.addNewCharacter(' ');
		for (const char& c : e)
			b.addNewCharacter(c);
	}
	for (int i = 0; i < m - 1; ++i) {
		if (!a.empty())
			a.addNewCharacter(' ');
		for (const char& c : s[i])
			a.addNewCharacter(c);
	}
	for (int i = m - 1; i < n; ++i) {
		if (!a.empty())
			a.addNewCharacter(' ');
		for (const char& c : s[i])
			a.addNewCharacter(c);
		if (a.getHashedValue() == b.getHashedValue())
			return true;
		for (const char& c : s[i - m + 1])
			a.popFirstCharacter();
		if (!a.empty())
			a.popFirstCharacter(); // remove white space
	}
	return false;
};

string toLowerString(string s) {
	for (char& c : s)
		c = tolower(c);
	return s;
};