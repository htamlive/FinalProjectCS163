#include <iostream>
#include <thread>
#include "DataExecution.h"
#include "Dictionary.h"
#include <string>
#include <cstring>

using namespace sf;
using namespace tgui;
using namespace std;

void runProgram() {
	Dictionary dictionary;
	dictionary.run();
	DataExecution* dataExecution = &DataExecution::getInstance();
	for (auto i : { 0, 2 }) {
		dataExecution->saveAndRemoveTrie(i, "Keys");
		dataExecution->saveAndRemoveTrie(i, "Defs");
	}
};

void loadData() {
	DataExecution* dataExecution = &DataExecution::getInstance();
	for (auto i : {0, 1, 2, 4 }) dataExecution->loadKeys(i);
	for (auto i : {0, 1, 2, 4 }) dataExecution->loadDefs(i);
	
	while (!dataExecution->getShutDown()) {
		if (dataExecution->getReload()) {
			dataExecution->setReload(false);
			dataExecution->restore();
		}
	}
	dataExecution->saveAndRemoveTrie(3, "Keys");
	dataExecution->saveAndRemoveTrie(3, "Defs");
}

void loadBig() {
	DataExecution* dataExecution = &DataExecution::getInstance();
	dataExecution->loadKeys(3);
	dataExecution->loadDefs(3);
	while (!dataExecution->getShutDown()) {
		if (dataExecution->getReload()) {
			dataExecution->setReload(false);
			dataExecution->restore();
		}
	}
	for (auto i : { 1, 4 }) {
		dataExecution->saveAndRemoveTrie(i, "Keys");
		dataExecution->saveAndRemoveTrie(i, "Defs");
	}
	

}

#include "QueueDataStructure.h"

int main() {
	std::thread work3(loadBig);
	std::thread work1(loadData);
	std::thread work2(runProgram);

	work1.join();
	work2.join();
	work3.join();
	return 0;
}

