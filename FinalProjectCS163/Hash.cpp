#include "Hash.h"

HashedString::HashedString(const int base, const int MOD) {
	this -> base = base;
	this -> MOD = MOD;
	this -> value = 0;
	(this->power).push_back(1);
};

void HashedString::addNewCharacter(const char c) {
	(this->characters).push_back(c);
	(this->power).push_back(1LL * (this->power).back() * c % MOD);
	(this->value) = (1LL * (this -> value) * base % (this -> MOD) + c) % (this -> MOD);
};

int HashedString::getHashedValue() const {
	return this -> value;
};