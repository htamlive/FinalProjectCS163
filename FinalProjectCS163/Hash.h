#pragma once

#include <vector>
#include <cassert>

using namespace std;

class HashedString {
private:
	
	int value, base, MOD;
	vector<char> characters;
	vector<int> power;

public:
	
	HashedString(const int base, const int MOD);
	
	friend bool operator == (const HashedString& a, const HashedString& b) {
		assert(a.base == b.base && a.MOD == b.MOD);
		return a.value == b.value;
	};

	void addNewCharacter(const char c);
	int getHashedValue() const;
};