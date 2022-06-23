#include "Hash.h"
#include "helper.h"

HashedString::HashedString(const int base, const int MOD) {
	this -> base = base;
	this -> MOD = MOD;
	this -> value = 0;
	(this->power).push_back(1);
};

HashedString::HashedString() {
	this->base = HashedString::getRandomBase();
	this->MOD = HashedString::getRandomModulo();
	this->value = 0;
	(this->power).push_back(1);
};

void HashedString::addNewCharacter(const int c) {
	(this->characters).push(c);
	(this->power).push_back(1LL * (this->power).back() * c % MOD);
	(this->value) = (1LL * (this -> value) * base % (this -> MOD) + c) % (this -> MOD);
};

int HashedString::getHashedValue() const {
	return this -> value;
};

int HashedString::getRandomModulo() {
	const static vector<int> listOfModulos({ 1000000123, 1000001011, 1000007707, 1000009009, 1000009099, 1000009999, 1000010101, 1000011011 });
	return getRandomNumberFromList(listOfModulos);
}

int HashedString::getRandomBase() {
	const static vector<int> listOfBases({311, 1009, 1777, 1999, 2003, 2777, 2999, 4999, 7001});
	return getRandomNumberFromList(listOfBases);
}

bool HashedString::empty() const {
	return (this->characters).empty();
};

bool HashedString::popFirstCharacter() {
	if ((this->characters).empty())
		return false;
	(this->power).pop_back();
	(this->value) = ((this->value) - 1LL * (this->characters).front() * (this->power).back() % (this->MOD) + (this->MOD)) % (this->MOD);
	(this->characters).pop();
	return true;
}