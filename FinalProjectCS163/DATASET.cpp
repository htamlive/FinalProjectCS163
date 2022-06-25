#include "DATASET.h"

void DATASET::loadData() {
	if (_typeOfdata == DATASETID::EMOJI) {
		loadFromEmotional();
		return;
	}
	if (_typeOfdata == DATASETID::SLANG) {
		loadFromSlang();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		loadFromCSV();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoEN) {
		loadFromOxford();
		return;
	}
}

void DATASET::saveData() {
	if (_typeOfdata == DATASETID::EMOJI) {
		saveToEmotional();
		return;
	}
	if (_typeOfdata == DATASETID::SLANG) {
		saveToSlang();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		saveToCSV();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoEN) {
		saveToOxford();
		return;
	}
}

void DATASET::addWord(pair<string, string> newWord) {
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::SLANG) {
		Data.push_back(newWord);
		return;
	}
	else if (_typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN || _typeOfdata == DATASETID::ENtoEN) {
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
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		Data[id].first = "";
		Data[id].second = "";
		return;
	}
	else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
		Data[id].first = "";
		Data[id].second = "";
		Core_Data[id] = "";
		return;
	}
}

pair<string, string> DATASET::getData(int id) const {
	if (id > Data.size() || Data[id].first == "") {
		cout << id << "\n";
		cerr << "Error index" << endl;

		return make_pair("", "");
	}
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		return Data[id];
	}
	else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
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
			fout << i.first << ',' << i.second << endl;
		}
	}
	fout.close();
}

void DATASET::loadFromEmotional() {
	ifstream fin(dataset_name);
	string temp1, temp2;
	while (!fin.eof()) {
		getline(fin, temp1, '\t');
		getline(fin, temp2);
		if (fin.fail())
			break;
		temp2.erase(remove(temp2.begin(), temp2.end(), '\t'), temp2.end());
		Data.push_back(make_pair(temp1, temp2));
	}
	fin.close();
}

void DATASET::saveToEmotional() {
	ofstream fout(dataset_name);
	for (auto i : Data) {
		fout << i.first << '\t' << i.second << endl;
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
		if (fin.fail())
			break;
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
			fout << Data[i].first << '`' << Core_Data[i] << endl;
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
			fout << Data[i].first << ',' << Core_Data[i] << endl;
		}
	}
	fout.close();
}

void DATASET::restoreDictionary() {
	this->Core_Data.clear();
	this->Data.clear();
	if (_typeOfdata == DATASETID::EMOJI) {
		ifstream src("OrgData/emotional.txt", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		src.close();
		dst.close();
		loadFromEmotional();
		return;
	}
	if (_typeOfdata == DATASETID::SLANG) {
		ifstream src("OrgData/slang.txt", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		src.close();
		dst.close();
		loadFromSlang();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoVIE) {
		ifstream src("OrgData/FilterENtoVIEAgain.csv", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		src.close();
		dst.close();
		loadFromCSV();
		return;
	}
	if (_typeOfdata == DATASETID::ENtoEN) {
		ifstream src("OrgData/FilterOxford.csv", ios::binary);
		ofstream dst(dataset_name, ios::binary);
		dst << src.rdbuf();
		src.close();
		dst.close();
		loadFromOxford();
		return;
	}
}
