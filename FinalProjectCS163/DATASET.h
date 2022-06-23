#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
#include <random>
#include <ctime>
using namespace std;

enum DATASETID
{
	EMOJI = 0,
	SLANG = 1,
	ENtoVIE = 2,
	ENtoEN = 3,
	VIEtoEN = 4
};

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
			_typeOfdata = DATASETID::EMOJI;
		}
		if (dataset_name == "Dataset/slang.txt") {
			_typeOfdata = DATASETID::SLANG;
		}
		if (dataset_name == "Dataset/FilterENtoVIEAgain.csv") {
			_typeOfdata = DATASETID::ENtoVIE;
		}
		if (dataset_name == "Dataset/FilterOxford.csv") {
			_typeOfdata = DATASETID::ENtoEN;
		}
	}

	DATASET(int id) {
		_typeOfdata = id;

		switch (this->_typeOfdata)
		{
		case DATASETID::EMOJI:
			dataset_name = "Dataset/emotional.txt";
			break;
		case DATASETID::SLANG:
			dataset_name = "Dataset/slang.txt";
			break;
		case DATASETID::ENtoVIE:
			dataset_name = "Dataset/FilterENtoVIEAgain.csv";
			break;
		case DATASETID::ENtoEN:
			dataset_name = "Dataset/FilterOxford.csv";
			break;
		case DATASETID::VIEtoEN:
			dataset_name = "Dataset/FilterENtoVIEAgain.csv";
			break;
		default:
			break;
		}
	}

	void swap() {
		for (auto& x : this->Data) {
			string se = x.first;
			x.first = x.second;
			x.second = se;
		}
	}

	vector<int> getRand(int tot) {
		vector<int> res;
		srand(time(NULL));
		while (res.size() < tot) {
			//
			long long tmp = 1ll * rand() * rand() % (1000000007) % this->Data.size();
			bool flag = true;
			for (auto x : res) {
				if (x == tmp) {
					flag = false;
					break;
				}
			}
			if(flag) res.push_back(tmp);
		}
		return res;
	}

	void loadData();
	void saveData();

	void addWord(pair<string, string> newWord);
	void removeWord(int id);

	void restoreDictionary();

	pair<string, string> getData(int id) const;
};

