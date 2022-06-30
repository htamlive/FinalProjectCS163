#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>

using namespace std;

#include "helper.h"
#include "Trie.h"
#include "Hash.h"

vector<string> splitString(const string& s) {
	//Split string by space
	vector<string> result;
	string t;

	stringstream ss(s);

	while (ss >> t) {
		result.push_back(t);
	}
	
	return result;
};

vector<string> splitString(const string& text, const string& seperate) {
	const int lengthOfText = text.size(), lengthOfDelimiter = seperate.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	vector<long long> power(1, 1), h(1, 0);
	long long hashedDelimiter = 0;
	vector<string> result;
	int i = 0, j = 0, k;
	for (int i = 1; i <= lengthOfText; ++i) {
		power.push_back(power.back() * base % MOD);
		h.push_back((h.back() * base % MOD + text[i - 1]) % MOD);
	}
	for (const char& c : seperate)
		hashedDelimiter = (hashedDelimiter * base % MOD + c) % MOD;
	while (i < lengthOfText) {
		k = i + lengthOfDelimiter - 1;
		if (k < lengthOfText && (h[k + 1] - h[i] * power[lengthOfDelimiter] % MOD + MOD) % MOD == hashedDelimiter) {
			result.push_back(text.substr(j, i - j));
			j = i = k + 1;
		} else
			++i;
	}
	if (j < lengthOfText)
		result.push_back(text.substr(j, lengthOfText));
	return result;
};

vector<string> splitString(const string& text, const string& seperate, int maximum) {
	const int lengthOfText = text.size(), lengthOfDelimiter = seperate.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	vector<long long> power(1, 1), h(1, 0);
	long long hashedDelimiter = 0;
	vector<string> result;
	int i = 0, j = 0, k;
	for (int i = 1; i <= lengthOfText; ++i) {
		power.push_back(power.back() * base % MOD);
		h.push_back((h.back() * base % MOD + text[i - 1]) % MOD);
	}
	for (const char& c : seperate)
		hashedDelimiter = (hashedDelimiter * base % MOD + c) % MOD;
	while (i < lengthOfText) {
		k = i + lengthOfDelimiter - 1;
		if (maximum > 0 && k < lengthOfText && (h[k + 1] - h[i] * power[lengthOfDelimiter] % MOD + MOD) % MOD == hashedDelimiter) {
			result.push_back(text.substr(j, i - j));
			j = i = k + 1;
			--maximum;
		}
		else
			++i;
	}
	if (j < lengthOfText)
		result.push_back(text.substr(j, lengthOfText));
	return result;
};

string replaceString(const string& text, const string& oldSubstring, const string& newSubstring) {
	const int lengthOfText = text.size(), lengthOfOldSubstring = oldSubstring.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	vector<long long> power(1, 1), h(1, 0);
	long long hashedOldSubstring = 0;
	string result;
	int i = 0, k;
	for (int i = 1; i <= lengthOfText; ++i) {
		power.push_back(power.back() * base % MOD);
		h.push_back((h.back() * base % MOD + text[i - 1]) % MOD);
	}
	for (const char& c : oldSubstring)
		hashedOldSubstring = (hashedOldSubstring * base % MOD + c) % MOD;
	while (i < lengthOfText) {
		k = i + lengthOfOldSubstring - 1;
		if (k < lengthOfText && (h[k + 1] - h[i] * power[lengthOfOldSubstring] % MOD + MOD) % MOD == hashedOldSubstring) {
			result += newSubstring;
			i = k + 1;
		} else
			result += text[i++];
	}
	return result;
};

string replaceString(const string& text, const string& oldSubstring, const string& newSubstring, int maximum) {
	const int lengthOfText = text.size(), lengthOfOldSubstring = oldSubstring.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	vector<long long> power(1, 1), h(1, 0);
	long long hashedOldSubstring = 0;
	string result;
	int i = 0, k;
	for (int i = 1; i <= lengthOfText; ++i) {
		power.push_back(power.back() * base % MOD);
		h.push_back((h.back() * base % MOD + text[i - 1]) % MOD);
	}
	for (const char& c : oldSubstring)
		hashedOldSubstring = (hashedOldSubstring * base % MOD + c) % MOD;
	while (i < lengthOfText) {
		k = i + lengthOfOldSubstring - 1;
		if (maximum > 0 && k < lengthOfText && (h[k + 1] - h[i] * power[lengthOfOldSubstring] % MOD + MOD) % MOD == hashedOldSubstring) {
			result += newSubstring;
			i = k + 1;
			--maximum;
		} else
			result += text[i++];
	}
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

bool readInteger(const string& s, int &i, int& result) {
	const int length = s.size();
	if (i >= length)
		return false;
	bool negative;
	if (s[i] == '-') {
		negative = true;
		++i;
	} else
		negative = false;
	if (!isdigit(s[i]))
		return false;
	result = 0;
	while (i < length && isdigit(s[i]))
		(result *= 10) += s[i] - '0';
	if (negative)
		result *= -1;
	return true;
};