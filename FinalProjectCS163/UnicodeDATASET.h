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


	UnicodeDATASET(int id) {
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
			tgui::String se = x.first;
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

	void addWord(pair<tgui::String, tgui::String> newWord);
	void removeWord(int id);

	void restoreDictionary();

	pair<tgui::String, tgui::String> getData(int id) const;

	void deserialize() {
		string link = this->dataset_name.substr(0, dataset_name.length() - 3) + "bin";
		ifstream ifs("UnicodeData/Dataset/" + link, ios::binary);
		int32_t n;
		ifs.read((char*)&n, sizeof(int32_t));
		//assert(n % 2 == 0);
		vector<int16_t> tmp(n);
		ifs.read((char*)tmp.data(), n * sizeof(int16_t));
		tgui::String serial;
		for (const auto& x : tmp) {
			serial += char32_t(x);
		}
		ifs.close();
		loadFromSerial(serial);
	};

	void loadFromSerial(const tgui::String& serial) {
		Data.clear();
		Core_Data.clear();
		vector<tgui::String> list = serial.split(char32_t(9));
		if (list.size() % 2) list.pop_back();
		tgui::String key, def;

		for (int i = 0; i + 1 < list.size(); i += 2) {
			key = list[i];
			def = list[i + 1];
			Core_Data.push_back(def);
			if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::VIEtoEN) {
				tgui::String temp = "";
				for (auto i : def) {
					if (checkValidChar(i))
						temp += i;
				}
				Data.push_back(make_pair(key, temp));
			}
			else if (_typeOfdata == DATASETID::SLANG) {
				remove(def.begin(), def.end(), L'|');
				Data.push_back(make_pair(key, def));
			}
			else {
				Data.push_back(make_pair(key, def));
			}
		}
	}

	void addToList(const tgui::String& text, vector<int16_t>& list) {
		for (int i = 0; i < text.size(); ++i) {
			list.push_back(text[i]);
		}
	}

	void serialize() {
		vector<int16_t> tmp;
		if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::VIEtoEN) {
			for (int i = 0; i < Data.size(); i++) {
				addToList(Data[i].first, tmp);
				tmp.push_back(9);
				addToList(Core_Data[i], tmp);
				tmp.push_back(9);
			}
		}
		else {
			for (int i = 0; i < Data.size(); i++) {
				addToList(Data[i].first, tmp);
				tmp.push_back(9);
				addToList(Data[i].second, tmp);
				tmp.push_back(9);
			}
		}
		int32_t n = tmp.size();
		if(n%2) tmp.pop_back();
		string link = this->dataset_name.substr(0, dataset_name.length() - 3) + "bin";
		ofstream ofs("UnicodeData/Dataset/" + link, ios::binary);

		ofs.write((char*)&n, sizeof(int32_t));
		ofs.write((char*)tmp.data(), n * sizeof(int16_t));
		ofs.close();


	}
};

