#pragma once
#include <vector>
#include "Trie.h"
#include "helper.h"

class DataExecution
{
private:
	DATASET* datasets[5];
	Trie* trieKeys[5], *trieDefs[5];
	std::vector<int> favor[5];
	std::vector<int> history[5];

	bool finishDataset[5], finishKeys[5], finishDefs[5], isShutDown, isReload;

	bool loadSer = true;

	int curDataset;

	void addToTrieDefs(int id, bool isReload = false) {
		if (loadSer) {
			std::string name = this->datasets[id]->dataset_name;
			//cout << name << "\n";
			std::string link;
			if (!isReload) link = "Data/DataStructure/" + name.substr(0, name.length() - 4) + "Defs" + (string)".bin";
			else link = "Data/OrgData/DataStructure/" + name.substr(0, name.length() - 4) + "Defs" + (string)".bin";
			
			string s = getStringBin(link);
			this->trieDefs[id]->deserialize(s);
			cout << "sz " << s.length() << "\n";
		} else {
			for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
				pair<string, string> cur = this->datasets[id]->Data[j];
				transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
				const vector<string> words = splitString(cur.second);
				for (int i = 0; i < words.size(); ++i) {
					this->trieDefs[id]->addWord(words[i], { j, i });
				}
			}
		}
		std::cout << "Finish adding Defs of " << id << "\n";
		this->finishDefs[id] = true;
	}

	void addToTrieKeys(int id, bool isReload = false) {
		if (loadSer) {
			std::string name = this->datasets[id]->dataset_name;
			std::string link;
			if (!isReload) link = "Data/DataStructure/" + name.substr(0, name.length() - 4) + "Keys" + (string)".bin";
			else link = "Data/OrgData/DataStructure/" + name.substr(0, name.length() - 4) + "Keys" + (string)".bin";
			string s = getStringBin(link);
			this->trieKeys[id]->deserialize(s);
			cout << "sz " << s.length() << "\n";
		}
		else {
			for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
				pair<string, string> cur = this->datasets[id]->Data[j];
				this->trieKeys[id]->addWord(cur.first, { j, 0 });
			}
		}

		std::cout << "Finish adding Keys of " << id << "\n";
		this->finishKeys[id] = true;
	}

	void loadFavor(int id) {
		if (id < 0 || id > 4) return;
		std::ifstream ifs("Data/Favorite/FavoriteId" + std::to_string(id) + (string)".txt");
		if (!ifs.is_open()) return;
		int tot = 0;
		ifs >> tot;
		favor[id].resize(tot);
		for (int i = 0; i < tot; ++i) {
			ifs >> favor[id][i];
		}
		ifs.close();
	}

	void saveFavor(int id) {
		if (id < 0 || id > 4) return;
		std::ofstream ofs("Data/Favorite/FavoriteId" + std::to_string(id) + (string)".txt");
		int tot = favor[id].size();
		ofs << tot << "\n";
		for (int i = 0; i < tot; ++i) {
			ofs << favor[id][i] << "\n";
		}
		ofs.close();
	}

	void loadHistory(int id) {
		if (id < 0 || id > 4) return;
		string link;
		for (string i : { (string)"Data/History/History", (string)to_string(id), (string)".txt"}) {
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

	void saveHistory(int id) {
		if (id < 0 || id > 4) return;
		string link;
		for (string i : { (string)"Data/History/History", (string)to_string(id), (string)".txt"}) {
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


	
public:
	static DataExecution& getInstance() {
		static DataExecution instance;
		return instance;
	};

	DataExecution() {
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

	virtual ~DataExecution() {
		for (auto i : {0, 1, 2, 3, 4}) {
			
			if (this->trieKeys[i]) 
				saveAndRemoveTrie(i,"Keys");
			if (this->trieDefs[i]) 
				saveAndRemoveTrie(i, "Defs");

			if (this->datasets[i] && this->finishDataset[i]) {
				this->datasets[i]->saveData();
				std::cout << "Finish saving " << i << " " << datasets[i]->Data.size() << "\n";
				delete this->datasets[i];
			}
		}
		for (int i = 0; i < 5; ++i) {
			this->saveHistory(i);
			this->saveFavor(i);
		}

	}

	void saveAndRemoveTrie(int id, string additionStr) {

		std::string name = this->datasets[id]->dataset_name;
		std::string link = "Data/DataStructure/" + name.substr(0, name.length() - 4) + additionStr + (string)".bin";
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
		//cout << "finish " << id << " " << additionStr << " " << res.length() << "\n";
	}

	bool loadKeys(int id, bool isReload = false, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if (setCur) this->curDataset = id;
		if (this->trieKeys[id]) return false;

		this->trieKeys[id] = new Trie();
		loadDataset(id);
		addToTrieKeys(id, isReload);
		return true;
	}

	bool loadDefs(int id, bool isReload = false, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if (setCur) this->curDataset = id;
		if (this->trieDefs[id]) return false;

		this->trieDefs[id] = new Trie();
		loadDataset(id);
		addToTrieDefs(id, isReload);
		return true;
	}

	void addWord(pair<string, string> word) {
		this->datasets[this->curDataset]->addWord(word);
		pair<string, string> cur = this->datasets[this->curDataset]->Data.back();
		transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
		const vector<string> words = splitString(cur.second);
		int j = this->datasets[this->curDataset]->Data.size() - 1;
		for (int i = 0; i < words.size(); ++i) {
			this->trieDefs[this->curDataset]->addWord(words[i], { j, i });
		}
		this->trieKeys[this->curDataset]->addWord(cur.first, { j, 0 });
	}

	bool loadDataset(int id, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if(setCur) this->curDataset = id;
		if (this->datasets[id]) return false;
		this->datasets[id] = new DATASET(id);
		this->datasets[id]->loadData();
		this->finishDataset[id] = true;
		std::cout << "Finish load dataset of " << id << " size:" << this->datasets[id]->Data.size() <<  "\n";
		return true;
	}

	const vector<int>& getHistory(int type = -1) {
		if (type == -1) type = this->curDataset;
		if (type < 0 || type > 4) return {};
		
		return this->history[type];
	}

	void clearHistory(int type = -1) {
		if (type == -1) type = this->curDataset;
		string link;
		for (string i : { (string)"Data/History/History", (string)to_string(type), (string)".txt"}) {
			link += i;
		}
		std::ofstream ofs(link);
		if (!ofs.is_open()) return;
		ofs << 0;
		ofs.close();
	}

	bool isFavorite(int id) {
		auto it = find(favor[this->curDataset].begin(), favor[this->curDataset].end(), id);
		return (it != favor[this->curDataset].end());
	}

	int getCurSet() {
		return this->curDataset;
	}

	string getDefinition(string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		vector<pair<int, int>> def = this->trieKeys[this->curDataset]->getDefinitions(str);

		string defText = "";
		for (int i = 0; i < (int)def.size(); i++) {
			auto ans = this->datasets[this->curDataset]->getData(def[i].first);
			if (ans.second != "")
				defText +=  ans.second + "\n";
		}

		return defText;
	}

	vector<int> getID(string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		vector<pair<int, int>> def = this->trieKeys[this->curDataset]->getDefinitions(str);
		vector<int> ans;

		for (int i = 0; i < (int)def.size(); i++) {
			ans.push_back(def[i].first);
		}
		return ans;
	}

	void addFavoriteIDs(vector<int>& IDs) {
		for (int i = 0; i < (int)IDs.size(); i++) {
			this->favor[this->curDataset].push_back(IDs[i]);
		}
	}

	void addHistoryID(int ID) {
		this->removeHistoryIDs({ ID });
		this->history[this->curDataset].push_back(ID);
	}

	void removeFavoriteIDs(vector<int> IDs) {
		for (int i = 0; i < (int)IDs.size(); i++) {
			for (auto j = this->favor[this->curDataset].begin(); j != this->favor[this->curDataset].end(); j++) {
				if (*j == IDs[i]) {
					this->favor[this->curDataset].erase(j);
					break;
				}
			}
		}
	}

	void removeHistoryIDs(vector<int> IDs) {
		for (int i = 0; i < (int)IDs.size(); i++) {
			for (auto j = this->history[this->curDataset].begin(); j != this->history[this->curDataset].end(); j++) {
				if (*j == IDs[i]) {
					this->history[this->curDataset].erase(j);
					break;
				}
			}
		}
	}

	vector<int> getFavor(int id = -1) {
		if (id == -1) return this->favor[this->curDataset];
		if (id < 0 || id > 4) return {};
		return this->favor[id];
	}

	void setShutDown() {
		this->isShutDown = true;
	}

	bool getShutDown() {
		return this->isShutDown;
	}

	void setReload(bool flag) {
		this->isReload = flag;
	}

	bool getReload() {
		return this->isReload;
	}

	bool checkFinishAll() {
		for (int i = 0; i < 5; ++i) if (!finishDataset[i] || !finishKeys[i] || !finishDefs[i]) return false;
		return true;
	}

	vector<string> getListOfKeys(string prefix, int maximum) {
		//cerr << this->tries.size() << '\n';
		return this->trieKeys[this->curDataset]->getListOfWords(prefix, maximum);
	}

	vector<int> getRand(int tot) {
		return this->datasets[this->curDataset]->getRand(tot);
	}

	pair<string, string> getData(int id, int curSet = -1) {
		if(curSet == -1) return this->datasets[this->curDataset]->getData(id);
		return this->datasets[curSet]->getData(id);
	}

	int getCurDataset() {
		return this->curDataset;
	}

	void restore(int id = -1) {
		//this->setReload(true);
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

		loadKeys(id,true);
		loadDefs(id, true);
		
		//std::cout << "Finish restoring " << id << " " << this->datasets[id]->Data.size() << "\n";
	}

	vector<int> getKeys(string s, int maximum = 8) {
		return this->trieDefs[this->curDataset]->getKeys(*this->datasets[this->curDataset], s, maximum);
	}

	vector<int> getKeysSubarray(string s, int maximum = 8) {
		return this->trieDefs[this->curDataset]->getKeysSubarray(*this->datasets[this->curDataset], s,maximum);
	}

	vector<int> getKeysSubsequence(string s, int maximum = 8) {
		return this->trieDefs[this->curDataset]->getKeysSubsequence(*this->datasets[this->curDataset], s, maximum);
	}

	void removeWord(int id) {
		this->datasets[this->curDataset]->removeWord(id);
	}

};

