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

void DATASET::saveData() {
	if (_typeOfdata == 0) {
		saveToEmotional();
		return;
	}
	if (_typeOfdata == 1) {
		saveToSlang();
		return;
	}
	if (_typeOfdata == 2) {
		saveToCSV();
		return;
	}
	if (_typeOfdata == 3) {
		saveToOxford();
		return;
	}
}

void DATASET::addWord(pair<string, string> newWord) {
	if (_typeOfdata == 0 || _typeOfdata == 1) {
		Data.push_back(newWord);
		return;
	}
	else if (_typeOfdata == 2 || _typeOfdata == 3) {
		Data.push_back(newWord);
		Core_Data.push_back(newWord.second);
		return;
	}
}

void DATASET::removeWord(int id) {
	if (id > Data.size() || Data[id].first == "") {
		cerr << "Error index" << endl;
		return;
	}
	if (_typeOfdata == 0 || _typeOfdata == 2) {
		Data[id].first = "";
		Data[id].second = "";
		return;
	}
	else if (_typeOfdata == 1 || _typeOfdata == 3) {
		Data[id].first = "";
		Data[id].second = "";
		Core_Data[id] = "";
		return;
	}
}

pair<string, string> DATASET::getData(int id) const {
	if (id > Data.size() || Data[id].first == "") {
		cerr << "Error index" << endl;

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

void DATASET::saveToCSV() {
	ofstream fout(dataset_name);
	for (auto i : Data) {
		if (i.first != "") {
			fout << i.first << ',' << i.second << '\n';
		}
	}
	fout.close();
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

void DATASET::saveToEmotional() {
	ofstream fout(dataset_name);
	for (auto i : Data) {
		if (i.first != "") {
			fout << i.first << '\t' << i.second << '\n';
		}
	}
	fout.close();
}

void DATASET::loadFromSlang() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	getline(fin, temp1, '`');
	getline(fin, temp2, '\n');
	while (!fin.eof()) {
		getline(fin, temp1, '`');
		getline(fin, temp2, '\n');
		Core_Data.push_back(temp2);
		remove(temp2.begin(), temp2.end(), '|');
		Data.push_back(make_pair(temp1, temp2));
	}
	fin.close();
}

void DATASET::saveToSlang() {
	ofstream fout(dataset_name);
	fout << "Slag" << '`' << "Meaning" << '\n';
	for (int i = 0; i < Data.size(); i++) {
		if (Data[i].first != "") {
			fout << Data[i].first << '`' << Core_Data[i] << '\n';
		}
	}
	fout.close();
}

void DATASET::loadFromOxford() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	string targ;
	getline(fin, temp1, ',');
	getline(fin, temp2, '\n');
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

void DATASET::saveToOxford() {
	ofstream fout(dataset_name);
	fout << "Keywords" << ',' << "Definitions" << '\n';
	for (int i = 0; i < Data.size(); i++) {
		if (Data[i].first != "") {
			fout << Data[i].first << ',' << Core_Data[i] << '\n';
		}
	}
	fout.close();
}

void DATASET::restoreDictionary() {
	if (_typeOfdata == DATASETID::EMOJI) {
		ifstream src("OrgData/emotional.txt", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		loadFromEmotional();
		return;
	}
	if (_typeOfdata == DATASETID::SLANG) {
		ifstream src("OrgData/slang.txt", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		loadFromSlang();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoVIE) {
		ifstream src("OrgData/FilterENtoVIEAgain.csv", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		loadFromCSV();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoEN) {
		ifstream src("OrgData/FilterOxford.csv", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		loadFromOxford();
		return;
	}
}
