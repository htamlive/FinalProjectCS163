#include "DATASET.h"

void DATASET::saveData() {

}

void DATASET::loadData() {
	if (_typeOfdata == 0) {
		loadFromEmotional();
	}
	else if (_typeOfdata == 1) {
		loadFromSlang();
	}
	else if (_typeOfdata == 2) {
		loadFromCSV();
	}
	else {
		cout << "ERROR FILE TYPE" << endl;
	}
}

void DATASET::addWord() {

}

void DATASET::removeWord() {

}

pair<string, string> DATASET::getData(int id) {

}

void DATASET::loadFromCSV() {
	ifstream fin(dataset_name, ios::in);
	string temp1;
	string temp2;
	getline(fin, temp1, ',');
	getline(fin, temp2, ',');
	//push data to vector, init mark_set to check availability
	while (!fin.eof()) {
		getline(fin, temp1, ',');
		getline(fin, temp2, ',');
		Data.push_back(make_pair(temp1, Split_string(temp2, " ")));
		_markSET.push_back(true);
	}
	fin.close();
	cout << "Success CSV loading" << endl;
}

void DATASET::loadFromEmotional() {
	ifstream fin(dataset_name, ios::in);
	string temp1;
	string temp2;
	fin >> temp1 >> temp2;
	while (!fin.eof()) {
		fin >> temp1 >> temp2;
		Data.push_back(make_pair(temp1, Split_string(temp2, " ")));
		_markSET.push_back(true);
	}
	fin.close();
}

void DATASET::loadFromSlang() {
	ifstream fin(dataset_name, ios::in);
	string temp;
	while (!fin.eof()) {
		getline(fin, temp, '\n');
		vector<string> v_temp = Split_string(temp, "`");
		Data.push_back(make_pair(v_temp[0], Split_string(v_temp[1], "| ")));
		_markSET.push_back(true);
	}
	fin.close();
}

vector<string> DATASET::Split_string(string s, string split_type) {
	stringstream SS(s);
	regex split(split_type);
	sregex_token_iterator begin(s.begin(), s.end(), split), end;
	vector<string> tokens(begin, end);
	return tokens;
}
