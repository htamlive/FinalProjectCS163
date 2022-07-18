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


class UnicodeDATASET
{
private:
	//type -1 <=> default
	int _typeOfdata = -1;

public:
	vector<pair<tgui::String, tgui::String>> Data;
	vector<tgui::String> Core_Data;
	string dataset_name;


	UnicodeDATASET(int id);

	void swap();

	vector<int> getRand(int tot);

	void loadData();
	void saveData();

	void addWord(pair<tgui::String, tgui::String> newWord);
	void removeWord(int id);

	void restoreDictionary();

	pair<tgui::String, tgui::String> getData(int id) const;

	void deserialize();
	
	void loadFromSerial(const tgui::String& serial);

	void addToList(const tgui::String& text, vector<int16_t>& list);

	void serialize();
};

