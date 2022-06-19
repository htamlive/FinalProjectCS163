#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <regex>
using namespace std;

class DATASET
{
private:
	//type -1 <=> default
	int _typeOfdata = -1;
	vector<bool> _markSET;
	//type 0
	void loadFromCSV();
	//type 1
	void loadFromEmotional();
	//type 2
	void loadFromSlang();

	vector<string> Split_string(string s, string split_type);
public:
	vector < pair<string, vector<string>>> Data;
	string dataset_name;

	DATASET(string dataname) {
		dataset_name = dataname;
		if (dataset_name == "slang.txt") {
			_typeOfdata = 0;
		}
		else if (dataset_name == "emotional.txt") {
			_typeOfdata == 1;
		}
		else if (dataset_name == "FilterENtoVIEAgain.csv") {
			_typeOfdata = 2;
		}
	}

	void saveData();
	void loadData();

	void addWord();
	void removeWord();
	
	//pair<string, vector<string> > getData(int id);
	//pair<string, string> getData(int id);
	pair<string, vector<string> > getDataByIDofLine(int id) const;
};

