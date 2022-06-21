#include <iostream>
#include <thread>
#include "DataExecution.h"
#include "Dictionary.h"

using namespace sf;
using namespace tgui;
using namespace std;

void runProgram() {
	Dictionary dictionary;
	dictionary.run();
};

void loadData() {
	DataExecution* dataExecution = &DataExecution::getInstance();
	for (auto i : {0, 1, 2, 4}) dataExecution->loadKeys(i);
	for (auto i : { 0, 1, 2, 4 }) dataExecution->loadDefs(i);
}

void loadBig() {
	DataExecution* dataExecution = &DataExecution::getInstance();
	dataExecution->loadKeys(3);
	dataExecution->loadDefs(3);
}

int main() {
	std::thread work1(loadData);
	std::thread work2(runProgram);
	std::thread work3(loadBig);
	
	work1.join();
	work2.join();
	work3.join();
	return 0;
}

