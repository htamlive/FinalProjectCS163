#include "DATASET.h"

void DATASET::loadData() {
	wifstream fin("Data/Dataset/" + dataset_name);
	wstring key, def;
	if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::VIEtoEN) {
		while (!fin.eof()) {
			getline(fin, key, L'\t');
			getline(fin, def, L'\t');
			if (fin.fail())
				break;
			Core_Data.push_back(def);
			wstring temp = L"";
			for (auto i : def) {
				if (isalnum((unsigned char)i) || isspace((unsigned char)i)) {
					temp += i;
				}
			}
			Data.push_back(make_pair(key, temp));
		}
	}
	else if (_typeOfdata == DATASETID::SLANG) {
		while (!fin.eof()) {
			getline(fin, key, L'\t');
			getline(fin, def, L'\t');
			Core_Data.push_back(def);
			if (fin.fail()) {
				break;
			}
			remove(def.begin(), def.end(), L'|');
			Data.push_back(make_pair(key, def));
		}
	}
	else {
		while (!fin.eof()) {
			getline(fin, key, L'\t');
			getline(fin, def, L'\t');
			if (fin.fail())
				break;
			Data.push_back(make_pair(key, def));
		}
	}
	fin.close();
}

void DATASET::saveData() {
	wofstream fout("Data/Dataset/" + dataset_name);
	if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::VIEtoEN) {
		for (int i = 0; i < Data.size(); i++) {
			fout << Data[i].first << '\t' << Core_Data[i] << '\t';
		}
	}
	else {
		for (int i = 0; i < Data.size(); i++) {
			fout << Data[i].first << '\t' << Data[i].second << '\t';
		}
	}
	fout.close();
}

void DATASET::addWord(pair<wstring, wstring> newWord) {
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
	if (id > Data.size() || Data[id].first == L"") {
		cerr << "Error index" << endl;
		return;
	}
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		Data[id].first = L"";
		Data[id].second = L"";
		return;
	}
	else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
		Data[id].first = L"";
		Data[id].second = L"";
		Core_Data[id] = L"";
		return;
	}
}

pair<wstring, wstring> DATASET::getData(int id) const {

	if (id > Data.size() || Data[id].first == L"") {
		cout << id << "\n";
		cerr << "Error index" << endl;

		return make_pair(L"", L"");
	}
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		return Data[id];
	}
	else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
		return make_pair(Data[id].first, Core_Data[id]);
	}
	return make_pair(L"", L"");
}

void DATASET::restoreDictionary() {
	this->Core_Data.clear();
	this->Data.clear();
	ifstream src("Data/OrgData/Dataset/" + dataset_name, ios::binary);
	ofstream dst("Data/OrgData/Dataset/" + dataset_name, ios::binary);
	dst << src.rdbuf();
	loadData();
	src.close();
	dst.close();
}
