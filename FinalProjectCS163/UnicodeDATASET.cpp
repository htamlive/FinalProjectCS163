#include "UnicodeDATASET.h"

UnicodeDATASET::UnicodeDATASET(int id) {
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

void UnicodeDATASET::swap() {
	for (auto& x : this->Data) {
		tgui::String se = x.first;
		x.first = x.second;
		x.second = se;
	}
}

vector<int> UnicodeDATASET::getRand(int tot) {
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

void UnicodeDATASET::loadData() {
	wifstream fin("UnicodeData/Dataset/" + dataset_name);
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
				if (checkValidChar(i))
					temp += i;
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

void UnicodeDATASET::saveData() {
	wofstream fout("UnicodeData/Dataset/" + dataset_name);
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

void UnicodeDATASET::addWord(pair<tgui::String, tgui::String> newWord) {
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

void UnicodeDATASET::removeWord(int id) {
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

pair<tgui::String, tgui::String> UnicodeDATASET::getData(int id) const {

	if (id > Data.size() || Data[id].first == "") {
		cout << id << "\n";
		cerr << "Error index" << endl;

		return make_pair("", "");
	}
	if (_typeOfdata == DATASETID::EMOJI || _typeOfdata == DATASETID::ENtoVIE || _typeOfdata == DATASETID::VIEtoEN) {
		return make_pair(Data[id].first, Data[id].second);
	}
	else if (_typeOfdata == DATASETID::SLANG || _typeOfdata == DATASETID::ENtoEN) {
		return make_pair(Data[id].first, Core_Data[id]);
	}
	return make_pair("", "");
}

void UnicodeDATASET::restoreDictionary() {
	this->Core_Data.clear();
	this->Data.clear();
	ifstream src("UnicodeData/OrgData/Dataset/" + dataset_name, ios::binary);
	ofstream dst("UnicodeData/Dataset/" + dataset_name, ios::binary);
	dst << src.rdbuf();
	loadData();
	src.close();
	dst.close();
}

void UnicodeDATASET::deserialize() {
	string link = this->dataset_name.substr(0, dataset_name.length() - 3) + "bin";
	ifstream ifs("UnicodeData/Dataset/" + link, ios::binary);
	int32_t n;
	ifs.read((char*)&n, sizeof(int32_t));
	vector<int16_t> tmp(n);
	ifs.read((char*)tmp.data(), n * sizeof(int16_t));
	tgui::String serial;
	for (const auto& x : tmp) {
		serial += char32_t(x);
	}
	ifs.close();
	loadFromSerial(serial);
};

void UnicodeDATASET::loadFromSerial(const tgui::String& serial) {
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

void UnicodeDATASET::addToList(const tgui::String& text, vector<int16_t>& list) {
	for (int i = 0; i < text.size(); ++i) {
		list.push_back(text[i]);
	}
}

void UnicodeDATASET::serialize() {
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
	if (n % 2) tmp.pop_back();
	string link = this->dataset_name.substr(0, dataset_name.length() - 3) + "bin";
	ofstream ofs("UnicodeData/Dataset/" + link, ios::binary);

	ofs.write((char*)&n, sizeof(int32_t));
	ofs.write((char*)tmp.data(), n * sizeof(int16_t));
	ofs.close();
}