#pragma once
#include "Trie.h"
#include "helper.h"

class DataExecution
{
private:
	DATASET* datasets[5];
	Trie* trieKeys[5], *trieDefs[5];
	int curDataset;
	void addToTrie(int id) {
		for (int j = 0; j < (int)this->datasets[id]->Data.size(); j++) {
			pair<string, string> cur = this->datasets[id]->Data[j];
			transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
			const vector<string> words = splitString(cur.second);
			for (int i = 0; i < words.size(); ++i) {
				this->trieDefs[id]->addWord(words[i], { j, i });
			}
			this->trieKeys[id]->addWord(cur.first, { j, 0 });
		}
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
		}

		this->curDataset = 0;
	}

	virtual ~DataExecution() {
		for (int i = 0; i < 5; ++i) {
			if (!this->datasets[i]) {
				this->datasets[i]->saveData();
				delete this->datasets[i];
			}

			if (!this->trieKeys[i]) {
				delete this->trieKeys[i];
			}
			if (!this->trieDefs[i]) {
				delete this->trieDefs[i];
			}
		}

	}

	bool load(int id) {
		if (id > 4 || id < 0) return false;
		this->curDataset = id;
		if (this->datasets[id]) return false;
		this->datasets[id] = new DATASET(id);
		this->datasets[id]->loadData();
		if (id == 4) {
			this->datasets[id]->swap();
		}
		this->trieKeys[id] = new Trie();
		this->trieDefs[id] = new Trie();
		this->addToTrie(id);
		return true;
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
			defText += "+" +  ans.second + "\n";
		}

		return defText;

	}

};

