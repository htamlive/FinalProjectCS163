#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include <map>

using namespace std;

#include "helper.h"
#include "Trie.h"
#include "UnicodeTrie.h"
#include "Hash.h"
#include <SFML/Network/Packet.hpp>

vector<string> splitString(const string& s) {
	//Split string by space
	stringstream ss(s);
	vector<string> result;
	string t;
	while (ss >> t) 
		result.push_back(t);
	return result;
};

vector<tgui::String> splitString(const tgui::String& s) {
	//Split string by space
	vector<tgui::String> result;
	tgui::String t;
	for (const auto& c : s) {
		if (c == ' ') {
			if (!t.empty()) {
				result.push_back(t);
				t.clear();
			}
			continue;
		} 
		t += c;
	}
	if (!t.empty())
		result.push_back(t);
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
		}
		else
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
		}
		else
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
		}
		else
			result += text[i++];
	}
	return result;
};

bool checkContainStrings(const vector<string>& s, const vector<string>& t) {
	//The characters of elements of s and t should be printable
	Trie trie;
	for (const string& e : s)
		trie.addWord(e, { 10, 10 });
	for (const string& e : t)
		if (!trie.containsWord(e))
			return false;
	return true;
};

bool checkContainStrings(const vector<tgui::String>& s, const vector<tgui::String>& t) {
	UnicodeTrie trie;
	for (const auto & e : s)
		trie.addWord(e, { 10, 10 });
	for (const auto & e : t)
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

bool checkContainStringsAsSubsequence(const vector<tgui::String>& s, const vector<tgui::String>& t) {
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

bool checkContainStringsAsSubarray(const vector<tgui::String>& s, const vector<tgui::String>& t) {
	if (t.empty())
		return true;
	const int n = s.size(), m = t.size(), base = HashedString::getRandomBase(), MOD = HashedString::getRandomModulo();
	if (n < m)
		return false;
	HashedString a(base, MOD), b(base, MOD);
	for (const auto & e : t) {
		if (!b.empty())
			b.addNewCharacter(' ');
		for (const auto & c : e)
			b.addNewCharacter(c);
	}
	for (int i = 0; i < m - 1; ++i) {
		if (!a.empty())
			a.addNewCharacter(' ');
		for (const auto & c : s[i])
			a.addNewCharacter(c);
	}
	for (int i = m - 1; i < n; ++i) {
		if (!a.empty())
			a.addNewCharacter(' ');
		for (const auto & c : s[i])
			a.addNewCharacter(c);
		if (a.getHashedValue() == b.getHashedValue())
			return true;
		for (const auto & c : s[i - m + 1])
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

bool turnNonUnicodeString(tgui::String& s) {
	if (s.length() == 0) return false;

	for (int i = 0; i < (int)s.size(); i++) {

		auto low = std::find(SOURCE_CHARACTERS.begin(), SOURCE_CHARACTERS.end(), s[i]);

		if (low == SOURCE_CHARACTERS.end()) {
			//cerr << "Yes your character suck\n";
			continue;
		}
		int pos = low - SOURCE_CHARACTERS.begin();
		//cerr << pos << ' ';
		s[i] = DESTINATION_CHARACTERS[pos];
	}
	//cerr << '\n';
	return true;
}

bool checkValidChar(int id)
{
	if (id >= 32 && id <= 127) return true;
	if (std::binary_search(SOURCE_CHARACTERS.begin(), SOURCE_CHARACTERS.end(), id)) return true;
	return false;
}

bool checkValidString(const tgui::String& s)
{
	for (const auto& x : s) {
		if (!checkValidChar(x)) return false;
	}
	return true;
}

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
		(result *= 10) += s[i++] - '0';
	if (negative)
		result *= -1;
	return true;
}

std::string getStringBin(std::string link) {
	std::ifstream ifs(link.c_str(), ios::binary | std::ios::ate);
	std::streamsize bufSz = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::string res;
	res.resize(bufSz);
	ifs.read((char*)res.data(), bufSz);
	ifs.close();
	return res;
}

tgui::String toLowerString(tgui::String s) {
	vector<int> m(10000, -1);
	m[258] = 259;
	m[7888] = 7889;
	m[272] = 273;
	m[7892] = 7893;
	m[296] = 297;
	m[416] = 417;
	m[7896] = 7897;
	m[7898] = 7899;
	m[7920] = 7921;
	m[7904] = 7905;
	m[7906] = 7907;
	m[360] = 361;
	m[7908] = 7909;
	m[431] = 432;
	m[7914] = 7915;
	m[7916] = 7917;
	m[7840] = 7841;
	m[7842] = 7843;
	m[7844] = 7845;
	m[7846] = 7847;
	m[7848] = 7849;
	m[7850] = 7851;
	m[7852] = 7853;
	m[7854] = 7855;
	m[7856] = 7857;
	m[7858] = 7859;
	m[7860] = 7861;
	m[7862] = 7863;
	m[7864] = 7865;
	m[7866] = 7867;
	m[7868] = 7869;
	m[7870] = 7871;
	m[7872] = 7873;
	m[192] = 224;
	m[193] = 225;
	m[194] = 226;
	m[195] = 227;
	m[7874] = 7875;
	m[200] = 232;
	m[201] = 233;
	m[202] = 234;
	m[7876] = 7877;
	m[204] = 236;
	m[205] = 237;
	m[7878] = 7879;
	m[7880] = 7881;
	m[210] = 242;
	m[211] = 243;
	m[212] = 244;
	m[213] = 245;
	m[7886] = 7887;
	m[217] = 249;
	m[218] = 250;
	m[7890] = 7891;
	m[221] = 253;
	m[7894] = 7895;
	m[7900] = 7901;
	m[7902] = 7903;
	m[7882] = 7883;
	m[7910] = 7911;
	m[7912] = 7913;
	m[7884] = 7885;
	m[7918] = 7919;
	for (auto& c : s) {
		if ('A' <= c && c <= 'Z')
			c = tolower(c);
		else {
			if (m[c] < 0)
				continue;
			c = m[c];
		}
	}
	return s;
};