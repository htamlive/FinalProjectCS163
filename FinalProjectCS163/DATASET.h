#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
using namespace std;

class DATASET
{
private:
	//type -1 <=> default
	int _typeOfdata = -1;
	//type 0
	void loadFromCSV();

	//type 1
	void loadFromEmotional();

	//type 2
	void loadFromSlang();

	//type 3
	void loadFromOxford();


public:
	vector<pair<string, string>> Data;
	vector<string> Core_Data;
	string dataset_name;

	DATASET(string dataname) {
		dataset_name = dataname;
		if (dataset_name == "dataset/emotional.txt") {
			_typeOfdata = 0;
		}
		if (dataset_name == "dataset/slang.txt") {
			_typeOfdata = 1;
		}
		if (dataset_name == "dataset/FilterENtoVIEAgain.csv") {
			_typeOfdata = 2;
		}
		if (dataset_name == "dataset/FilterOxford.csv") {
			_typeOfdata = 3;
		}
	}

	void loadData();

	void addWord();
	void removeWord();

	pair<string, string> getData(int id);
};

