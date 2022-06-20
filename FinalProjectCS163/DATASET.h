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
	void saveToCSV();
	//type 1
	void loadFromEmotional();
	void saveToEmotional();
	//type 2
	void loadFromSlang();
	void saveToSlang();
	//type 3
	void loadFromOxford();
	void saveToOxford();

public:
	vector<pair<string, string>> Data;
	vector<string> Core_Data;
	string dataset_name;

	DATASET(string dataname) {
		dataset_name = dataname;
		if (dataset_name == "Dataset/emotional.txt") {
			_typeOfdata = 0;
		}
		if (dataset_name == "Dataset/slang.txt") {
			_typeOfdata = 1;
		}
		if (dataset_name == "Dataset/FilterENtoVIEAgain.csv") {
			_typeOfdata = 2;
		}
		if (dataset_name == "Dataset/FilterOxford.csv") {
			_typeOfdata = 3;
		}
	}

	void loadData();
	void saveData();

	void addWord(pair<string, string> newWord);
	void removeWord(int id);

	void restoreDictionary();

	pair<string, string> getData(int id) const;
};

