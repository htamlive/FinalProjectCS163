#include "DATASET.h"
#include "helper.h"

DATASET::DATASET(int id) {
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

void DATASET::swap() {
	for (auto& x : this->Data) {
		string se = x.first;
		x.first = x.second;
		x.second = se;
	}
}

vector<int> DATASET::getRand(int tot) {
	vector<int> res;
	srand(time(NULL));
	while (res.size() < tot) {
		long long tmp = 1LL * rand() * rand() % (1000000007) % this->Data.size();
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

void DATASET::loadData() {
	ifstream fin("Data/Dataset/" + dataset_name);
	string key, def;
	if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::VIEtoEN) {
		while (!fin.eof()) {
			getline(fin, key, '\t');
			getline(fin, def, '\t');
			if (fin.fail())
				break;
			Core_Data.push_back(def);
			string temp = "";
			for (auto i : def) {
				if (checkValidChar(i)) 
					temp += i;
			}
			Data.push_back(make_pair(key, temp));
		}
	} else if (_typeOfdata == DATASETID::SLANG) {
		while (!fin.eof()) {
			getline(fin, key, '\t');
			getline(fin, def, '\t');
			Core_Data.push_back(def);
			if (fin.fail()) {
				break;
			}
			remove(def.begin(), def.end(), '|');
			Data.push_back(make_pair(key, def));
		}
	} else {
		while (!fin.eof()) {
			getline(fin, key, '\t');
			getline(fin, def, '\t');
			if (fin.fail())
				break;
			Data.push_back(make_pair(key, def));
		}
	}
	fin.close();
}

void DATASET::saveData() {
	ofstream fout("Dataset/" + dataset_name);
	if (_typeOfdata == DATASETID::ENtoEN || _typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::VIEtoEN) {
		for (int i = 0; i < Data.size(); i++) {
			fout << Data[i].first << '\t' << Core_Data[i] << '\t';
		}
	} else {
		for (int i = 0; i < Data.size(); i++) {
			fout << Data[i].first << '\t' << Data[i].second << '\t';
		}
	}
	fout.close();
}

void DATASET::addWord(pair<string, string> newWord) {
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::SLANG) {
		Data.push_back(newWord);
		return;
	} else if (_typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN || _typeOfdata == DATASETID::ENtoEN) {
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
	} else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
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
	} else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
		return make_pair(Data[id].first, Core_Data[id]);
	}
	return make_pair("", "");
}

void DATASET::restoreDictionary() {
	this->Core_Data.clear();
	this->Data.clear();
	ifstream src("Data/OrgData/Dataset/" + dataset_name, ios::binary);
	ofstream dst("Data/Dataset/" + dataset_name, ios::binary);
	dst << src.rdbuf();
	loadData();
	src.close();
	dst.close();
}

vector<pair<string, string> > DATASET::filterData(const function<bool(pair<string, string>)> &check) {
	vector<pair<string, string> > result;
	for (const auto &e : (this->Data))
		if (check(e))
			result.push_back(e);
	return result;
};