#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
#include <random>
#include <ctime>
#include <stdio.h>
#include <ctype.h>
#include <functional>

using namespace std;

#include "helper.h"

class DATASET
{
private:
	//type -1 <=> default
	int _typeOfdata = -1;

public:
	vector<pair<string, string>> Data;
	vector<string> Core_Data;
	string dataset_name;


	DATASET(int id);

	void swap();

	vector<int> getRand(int tot);

	void loadData();
	void saveData();

	void addWord(pair<string, string> newWord);
	void removeWord(int id);
	vector<pair<string, string> > filterData(const function<bool(pair<string, string>)> &check);

	void restoreDictionary();

	pair<string, string> getData(int id) const;
};
