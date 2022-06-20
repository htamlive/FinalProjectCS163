#include "DATASET.h"

void DATASET::loadData() {
	if (_typeOfdata == 0) {
		loadFromEmotional();
		return;
	}
	if (_typeOfdata == 1) {
		loadFromSlang();
		return;
	}
	if (_typeOfdata == 2) {
		loadFromCSV();
		return;
	}
	if (_typeOfdata == 3) {
		loadFromOxford();
		return;
	}
}

// void DATASET::addWord();

// void DATASET::removeWord();

pair<string, string> DATASET::getData(int id) {
	if (id > Data.size() || Data[id].first == "") {
		cout << "Error index" << endl;

		return make_pair("", "");
	}
	if (_typeOfdata == 0 || _typeOfdata == 1) {
		return Data[id];
	}
	else if (_typeOfdata == 2 || _typeOfdata == 3) {
		return make_pair(Data[id].first, Core_Data[id]);
	}
	return make_pair("", "");
}

void DATASET::loadFromCSV() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	while (!fin.eof()) {
		getline(fin, temp1, ',');
		getline(fin, temp2, '\n');
		Data.push_back(make_pair(temp1, temp2));
	}
	fin.close();
}

void DATASET::loadFromEmotional() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	while (!fin.eof()) {
		fin >> temp1;
		getline(fin, temp2, '\n');
		replace(temp2.begin(), temp2.end(), '.', ' ');
		Data.push_back(make_pair(temp1, temp2));
	}
	fin.close();
}

void DATASET::loadFromSlang() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	while (!fin.eof()) {
		getline(fin, temp1, '`');
		getline(fin, temp2, '\n');
		Core_Data.push_back(temp2);
		remove(temp2.begin(), temp2.end(), '|');
		Data.push_back(make_pair(temp1, temp2));
	}
}

void DATASET::loadFromOxford() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	string targ;
	while (!fin.eof()) {
		getline(fin, temp1, ',');
		getline(fin, temp2, '\n');
		targ = "";
		for (auto i : temp2) {
			if (isalnum(i) || isspace(i)) {
				targ += i;
			}
		}
		Data.push_back(make_pair(temp1, targ));
		Core_Data.push_back(temp2);
	}
	fin.close();
}
