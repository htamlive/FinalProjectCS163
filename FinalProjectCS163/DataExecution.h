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

	bool finish[5];

	int curDataset;

	void addToTrieDefs(int id) {
		for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
			pair<string, string> cur = this->datasets[id]->Data[j];
			transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
			const vector<string> words = splitString(cur.second);
			for (int i = 0; i < words.size(); ++i) {
				this->trieDefs[id]->addWord(words[i], { j, i });
			}
		}
		std::cout << "Finish adding Defs of " << id << "\n";
	}

	void addToTrieKeys(int id) {
		for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
			pair<string, string> cur = this->datasets[id]->Data[j];
			this->trieKeys[id]->addWord(cur.first, { j, 0 });
		}
		std::cout << "Finish adding Keys of " << id << "\n";
	}

	void loadFavor(int id) {
		if (id < 0 || id > 4) return;
		std::ifstream ifs("Dataset/FavoriteId" + std::to_string(id) + (string)".txt");
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
		std::ofstream ofs("Dataset/FavoriteId" + std::to_string(id) + (string)".txt");
		int tot = favor[id].size();
		ofs << tot << "\n";
		for (int i = 0; i < tot; ++i) {
			ofs << favor[id][i] << "\n";
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
			this->finish[i] = false;
		}

		this->curDataset = 0;
		for(int i = 0 ; i < 5; ++i)
			this->loadFavor(i);
	}

	virtual ~DataExecution() {
		for (auto i : {0, 1, 2, 3, 4}) {
			if (this->datasets[i] && this->finish[i]) {
				if(i < 4) this->datasets[i]->saveData();
				delete this->datasets[i];
			}

			if (this->trieKeys[i]) {
				delete this->trieKeys[i];
			}
			if (this->trieDefs[i]) {
				delete this->trieDefs[i];
			}
		}
		for (int i = 0; i < 5; ++i)
			this->saveFavor(i);

	}

	bool loadKeys(int id, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if (setCur) this->curDataset = id;
		if (this->trieKeys[id]) return false;

		this->trieKeys[id] = new Trie();
		loadDataset(id);
		addToTrieKeys(id);
	}

	bool loadDefs(int id, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if (setCur) this->curDataset = id;
		if (this->trieDefs[id]) return false;

		this->trieDefs[id] = new Trie();
		loadDataset(id);
		addToTrieDefs(id);
	}

	bool loadDataset(int id, bool setCur = false) {
		if (id > 4 || id < 0) return false;
		if(setCur) this->curDataset = id;
		if (this->datasets[id]) return false;
		this->datasets[id] = new DATASET(id);
		this->datasets[id]->loadData();
		if (id == 4) {
			this->datasets[id]->swap();
		}
		this->finish[id] = true;
		std::cout << "Finish load dataset of " << id << "\n";
		return true;
	}

	vector<int> loadHistory(int type) {
		if (type < 0 || type > 4) return {};
		string link;
		for (string i : { (string)"Dataset/History", (string)to_string(type), (string)".txt"}) {
			link += i;
		}
		std::ifstream ifs(link);
		if (!ifs.is_open()) return {};
		int tot;
		ifs >> tot;
		vector<int> res(tot);
		for (int i = 0; i < tot; ++i) {
			ifs >> res[i];
		}

		ifs.close();
		return res;
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
			defText += "+ " +  ans.second + "\n";
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

	void restore(int id) {
		this->datasets[id]->restoreDictionary();
	}

	vector<pair<int, int>> getKeySubarray(string s) {
		return this->trieDefs[this->curDataset]->getKeySubarray(*this->datasets[this->curDataset], s);
	}

	vector<pair<int, int>> getKeySubsequence(string s) {
		return this->trieDefs[this->curDataset]->getKeySubsequence(*this->datasets[this->curDataset], s);
	}
};

