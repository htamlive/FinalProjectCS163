#include "DataExecution.h"

void DataExecution::addToTrieDefs(int id, bool isReload) {
	if (loadSer) {
		std::string name = this->datasets[id]->dataset_name;
		std::string link;
		if (!isReload) link = "UnicodeData/DataStructure/" + name.substr(0, name.length() - 4) + "Defs" + (string)".bin";
		else link = "UnicodeData/OrgData/DataStructure/" + name.substr(0, name.length() - 4) + "Defs" + (string)".bin";

		string s = getStringBin(link);
		this->trieDefs[id]->deserialize(s);
		cout << "sz " << s.length() << "\n";
	} else {
		for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
			pair< tgui::String, tgui::String> cur = this->datasets[id]->Data[j];
			transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
			const vector< tgui::String> words = splitUnicodeString(cur.second);
			for (int i = 0; i < words.size(); ++i) {
				this->trieDefs[id]->addWord(words[i], { j, i });
			}
		}
	}
	std::cout << "Finish adding Defs of " << id << "\n";
	this->finishDefs[id] = true;
}

void DataExecution::addToTrieKeys(int id, bool isReload) {
	if (loadSer) {
		std::string name = this->datasets[id]->dataset_name;
		std::string link;
		if (!isReload) link = "UnicodeData/DataStructure/" + name.substr(0, name.length() - 4) + "Keys" + (string)".bin";
		else link = "UnicodeData/OrgData/DataStructure/" + name.substr(0, name.length() - 4) + "Keys" + (string)".bin";
		string s = getStringBin(link);
		this->trieKeys[id]->deserialize(s);
		cout << "sz " << s.length() << "\n";
	} else {
		for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
			pair< tgui::String, tgui::String> cur = this->datasets[id]->Data[j];
			this->trieKeys[id]->addWord(cur.first, { j, 0 });
		}
	}

	std::cout << "Finish adding Keys of " << id << "\n";
	this->finishKeys[id] = true;
}

void DataExecution::loadFavor(int id) {
	if (id < 0 || id > 4) return;
	std::ifstream ifs("UnicodeData/Favorite/FavoriteId" + std::to_string(id) + (string)".txt");
	if (!ifs.is_open()) return;
	int tot = 0;
	ifs >> tot;
	favor[id].resize(tot);
	for (int i = 0; i < tot; ++i) {
		ifs >> favor[id][i];
	}
	ifs.close();
}

void DataExecution::saveFavor(int id) {
	if (id < 0 || id > 4) return;
	std::ofstream ofs("UnicodeData/Favorite/FavoriteId" + std::to_string(id) + (string)".txt");
	int tot = favor[id].size();
	ofs << tot << "\n";
	for (int i = 0; i < tot; ++i) {
		ofs << favor[id][i] << "\n";
	}
	ofs.close();
}

void DataExecution::loadHistory(int id) {
	if (id < 0 || id > 4) return;
	string link;
	for (string i : { (string)"UnicodeData/History/History", (string)to_string(id), (string)".txt"}) {
		link += i;
	}
	std::ifstream ifs(link);
	if (!ifs.is_open()) return;
	int tot;
	ifs >> tot;
	this->history[id].resize(tot);
	for (int i = 0; i < tot; ++i) {
		ifs >> history[id][i];
	}

	ifs.close();
}

void DataExecution::saveHistory(int id) {
	if (id < 0 || id > 4) return;
	string link;
	for (string i : { (string)"UnicodeData/History/History", (string)to_string(id), (string)".txt"}) {
		link += i;
	}
	std::ofstream ofs(link);
	if (!ofs.is_open()) return;
	ofs << min((int)this->history[id].size(), 8) << "\n";

	int start = max(0, (int)history[id].size() - 8);
	for (int i = start; i < history[id].size(); ++i) {
		ofs << this->history[id][i] << "\n";
	}

	ofs.close();
}

DataExecution& DataExecution::getInstance() {
	static DataExecution instance;
	return instance;
};

DataExecution::DataExecution() {
	for (int i = 0; i < 5; ++i) {
		this->datasets[i] = nullptr;
		this->trieKeys[i] = nullptr;
		this->trieDefs[i] = nullptr;
		this->finishDataset[i] = false;
		this->finishKeys[i] = false;
		this->finishDefs[i] = false;

	}
	this->isShutDown = false;
	this->curDataset = 0;
	for (int i = 0; i < 5; ++i) {
		this->loadFavor(i);
		this->loadHistory(i);
	}
}

DataExecution::~DataExecution() {
	for (auto i : { 0, 1, 2, 3, 4 }) {

		if (this->trieKeys[i])
			saveAndRemoveTrie(i, "Keys");
		if (this->trieDefs[i])
			saveAndRemoveTrie(i, "Defs");

		if (this->datasets[i] && this->finishDataset[i]) {
			this->datasets[i]->serialize();
			std::cout << "Finish saving " << i << " " << datasets[i]->Data.size() << "\n";
			delete this->datasets[i];
		}
	}
	for (int i = 0; i < 5; ++i) {
		this->saveHistory(i);
		this->saveFavor(i);
	}

}

void DataExecution::saveAndRemoveTrie(int id, string additionStr) {
	std::string name = this->datasets[id]->dataset_name;
	std::string link = "UnicodeData/DataStructure/" + name.substr(0, name.length() - 4) + additionStr + (string)".bin";
	std::ofstream ofs(link, ios::binary);
	std::string res;
	if (additionStr == "Keys") {
		if (!this->trieKeys[id]) return;
		res = this->trieKeys[id]->serialize();
		delete this->trieKeys[id];
		this->trieKeys[id] = nullptr;
	}
	else {
		if (!this->trieDefs[id]) return;
		res = this->trieDefs[id]->serialize();
		delete this->trieDefs[id];
		this->trieDefs[id] = nullptr;
	}
	ofs.write(res.c_str(), res.length());
	ofs.close();
}

bool DataExecution::loadKeys(int id, bool isReload, bool setCur) {
	if (id > 4 || id < 0) return false;
	if (setCur) this->curDataset = id;
	if (this->trieKeys[id]) return false;

	this->trieKeys[id] = new UnicodeTrie();
	loadDataset(id);
	addToTrieKeys(id, isReload);
	return true;
}

bool DataExecution::loadDefs(int id, bool isReload, bool setCur) {
	if (id > 4 || id < 0) return false;
	if (setCur) this->curDataset = id;
	if (this->trieDefs[id]) return false;

	this->trieDefs[id] = new UnicodeTrie();
	loadDataset(id);
	addToTrieDefs(id, isReload);
	return true;
}

void DataExecution::addWord(pair< tgui::String, tgui::String> word) {
	this->datasets[this->curDataset]->addWord({ word.first, word.second });

	pair<tgui::String, tgui::String> cur = this->datasets[this->curDataset]->Data.back();

	this->addHistoryID(this->datasets[this->curDataset]->Data.size() - 1);

	transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);

	const vector<tgui::String> words = splitUnicodeString(cur.second);

	int j = this->datasets[this->curDataset]->Data.size() - 1;
	for (int i = 0; i < words.size(); ++i) {
		this->trieDefs[this->curDataset]->addWord(words[i], { j, i });
	}
	this->trieKeys[this->curDataset]->addWord(cur.first, { j, 0 });

}

bool DataExecution::loadDataset(int id, bool setCur) {
	if (id > 4 || id < 0) return false;
	if (setCur) this->curDataset = id;
	if (this->datasets[id]) return false;
	this->datasets[id] = new UnicodeDATASET(id);
	this->datasets[id]->deserialize();
	this->finishDataset[id] = true;
	std::cout << "Finish load dataset of " << id << " size:" << this->datasets[id]->Data.size() << "\n";
	return true;
}

const vector<int>& DataExecution::getHistory(int type) {
	if (type == -1) type = this->curDataset;
	if (type < 0 || type > 4) return {};

	return this->history[type];
}

void DataExecution::clearHistory(int type) {
	if (type == -1) type = this->curDataset;
	string link;
	for (string i : { (string)"UnicodeData/History/History", (string)to_string(type), (string)".txt"}) {
		link += i;
	}
	std::ofstream ofs(link);
	if (!ofs.is_open()) return;
	ofs << 0;
	ofs.close();
}

bool DataExecution::isFavorite(int id) {
	auto it = find(favor[this->curDataset].begin(), favor[this->curDataset].end(), id);
	return (it != favor[this->curDataset].end());
}

int DataExecution::getCurSet() {
	return this->curDataset;
}

tgui::String DataExecution::getDefinition(tgui::String& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	vector<pair<int, int>> def = this->trieKeys[this->curDataset]->getDefinitions(str);

	tgui::String defText = "";
	for (int i = 0; i < (int)def.size(); i++) {
		auto ans = this->datasets[this->curDataset]->getData(def[i].first);
		if (ans.second != L"")
			defText += ans.second + L"\n";
	}

	return defText;
}

vector<int> DataExecution::getID(tgui::String& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	vector<pair<int, int>> def = this->trieKeys[this->curDataset]->getDefinitions(str);
	vector<int> ans;

	for (int i = 0; i < (int)def.size(); i++) {
		ans.push_back(def[i].first);
	}
	return ans;
}

void DataExecution::addFavoriteIDs(vector<int>& IDs) {
	for (int i = 0; i < (int)IDs.size(); i++) {
		this->favor[this->curDataset].push_back(IDs[i]);
	}
}

void DataExecution::addHistoryID(int ID) {
	this->removeHistoryIDs({ ID });
	this->history[this->curDataset].push_back(ID);
}

void DataExecution::removeFavoriteIDs(vector<int> IDs) {
	for (int i = 0; i < (int)IDs.size(); i++) {
		for (auto j = this->favor[this->curDataset].begin(); j != this->favor[this->curDataset].end(); j++) {
			if (*j == IDs[i]) {
				this->favor[this->curDataset].erase(j);
				break;
			}
		}
	}
}

void DataExecution::removeHistoryIDs(vector<int> IDs) {
	for (int i = 0; i < (int)IDs.size(); i++) {
		for (auto j = this->history[this->curDataset].begin(); j != this->history[this->curDataset].end(); j++) {
			if (*j == IDs[i]) {
				this->history[this->curDataset].erase(j);
				break;
			}
		}
	}
}

vector<int> DataExecution::getFavor(int id) {
	if (id == -1) return this->favor[this->curDataset];
	if (id < 0 || id > 4) return {};
	return this->favor[id];
}

void DataExecution::reloadFavor(int id, const vector<int>& x) {
	if (id == -1) id = this->curDataset;
	if (id < 0 || id > 4) return;
	this->favor[id] = x;
}

void DataExecution::reloadHistory(int id, const vector<int>& x) {
	if (id == -1) id = this->curDataset;
	if (id < 0 || id > 4) return;
	this->history[id] = x;
}

void DataExecution::setShutDown() {
	this->isShutDown = true;
}

bool DataExecution::getShutDown() {
	return this->isShutDown;
}

void DataExecution::setReload(bool flag) {
	this->isReload = flag;
}

bool DataExecution::getReload() {
	return this->isReload;
}

bool DataExecution::checkFinishAll() {
	for (int i = 0; i < 5; ++i) if (!finishDataset[i] || !finishKeys[i] || !finishDefs[i]) return false;
	return true;
}

vector<tgui::String> DataExecution::getListOfKeys(tgui::String prefix, int maximum) {
	return this->trieKeys[this->curDataset]->getListOfWords(prefix, maximum);
}

vector<int> DataExecution::getRand(int tot) {
	return this->datasets[this->curDataset]->getRand(tot);
}

pair<tgui::String, tgui::String> DataExecution::getData(int id, int curSet) {
	if (curSet == -1) curSet = this->curDataset;
	if (curSet < 0 || curSet > 4) return { L"", L"" };
	auto res = this->datasets[curSet]->getData(id);
	return this->datasets[curSet]->getData(id);
}

int DataExecution::getCurDataset() {
	return this->curDataset;
}

void DataExecution::restore(int id) {
	if (id == -1) id = this->curDataset;
	this->finishDataset[id] = false;
	this->finishKeys[id] = false;
	this->finishDefs[id] = false;

	this->datasets[id]->restoreDictionary();
	this->finishDataset[id] = true;
	delete this->trieDefs[id];
	delete this->trieKeys[id];


	this->trieDefs[id] = nullptr;
	this->trieKeys[id] = nullptr;

	loadKeys(id, true);
	loadDefs(id, true);
}

vector<int> DataExecution::getKeys(tgui::String s, int maximum) {
	return this->trieDefs[this->curDataset]->getKeys(*this->datasets[this->curDataset], s, maximum);
}

vector<int> DataExecution::getKeysSubarray(tgui::String s, int maximum) {
	return this->trieDefs[this->curDataset]->getKeysSubarray(*this->datasets[this->curDataset], s, maximum);
}

vector<int> DataExecution::getKeysSubsequence(tgui::String s, int maximum) {
	return this->trieDefs[this->curDataset]->getKeysSubsequence(*this->datasets[this->curDataset], s, maximum);
}

void DataExecution::removeWord(int id) {
	this->datasets[this->curDataset]->removeWord(id);
}

vector<tgui::String> DataExecution::filterFavor(const vector<tgui::String>& suggestedKeys) {
	vector<tgui::String> res;
	for (auto s : suggestedKeys) {
		bool flag = false;
		for (auto id : this->favor[this->curDataset]) {
			auto x = getData(id).first.toLower();
			if (s == x) {
				flag = true;
				break;
			}
		}
		if (flag) res.push_back(s);
	}
	return res;
}

vector<int> DataExecution::filterFavor(const vector<int>& suggestedIdx) {
	vector<int> res;
	for (auto s : suggestedIdx) {
		bool flag = false;
		for (auto id : this->favor[this->curDataset]) {
			if (s == id) {
				flag = true;
				break;
			}
		}
		if (flag) res.push_back(s);
	}
	return res;
}