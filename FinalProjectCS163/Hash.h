#pragma once

#include <vector>
#include <cassert>
#include <queue>
#include <iostream>

using namespace std;

#include "QueueDataStructure.h"

class HashedString {
private:
	
	int value, base, MOD;
	queue<int> characters;
	vector<int> power;

public:

	static int getRandomModulo();
	static int getRandomBase();
	
	HashedString(const int base, const int MOD);
	HashedString();
	
	friend bool operator == (const HashedString& a, const HashedString& b) {
		assert(a.base == b.base && a.MOD == b.MOD);
		return a.value == b.value;
	};

	friend bool operator != (const HashedString& a, const HashedString& b) {
		assert(a.base == b.base && a.MOD == b.MOD);
		return a.value != b.value;
	};

	void addNewCharacter(const int c);
	bool popFirstCharacter();
	int getHashedValue() const;
	bool empty() const;
};