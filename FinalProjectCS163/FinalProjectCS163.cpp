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
	Dictionary dictionary;
	dictionary.run();
	return 0;
}

