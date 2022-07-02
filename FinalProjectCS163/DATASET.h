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
#include "helper.h"
using namespace std;

class DATASET
{
private:
	//type -1 <=> default
	int _typeOfdata = -1;

public:
	vector<pair<string, string>> Data;
	vector<string> Core_Data;
	string dataset_name;


	DATASET(int id) {
		_typeOfdata = id;

		switch (this->_typeOfdata)
		{
		case DATASETID::EMOJI:
			dataset_name = "emotional.txt";
			break;
		case DATASETID::SLANG:
			dataset_name = "slang.txt";
			break;
		case DATASETID::ENtoVIE:
			dataset_name = "en-vi.txt";
			break;
		case DATASETID::ENtoEN:
			dataset_name = "en-en.txt";
			break;
		case DATASETID::VIEtoEN:
			dataset_name = "vi-en.txt";
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
			if (flag) res.push_back(tmp);
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
