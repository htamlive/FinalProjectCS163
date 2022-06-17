#include <iostream>
#include "Dictionary.h"

using namespace sf;
using namespace tgui;
using namespace std;

bool doSth() {
	cout << "Nice\n";
	return true;
}

int main() {
	Dictionary dict;
	dict.run();
	return 0;
}

