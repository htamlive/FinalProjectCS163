#pragma once
#include "Trie.h"
#include "helper.h"

class DataExecution
{
private:
	DATASET* datasets[5];
	Trie* trieKeys[5], *trieDefs[5];
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


	bool load(int id) {
		if (id > 4 || id < 0) return false;
		this->curDataset = id;
		if (this->datasets[id]) return false;
		this->datasets[id] = new DATASET(id);
		this->datasets[id]->loadData();
		if (id == 4) {
			this->datasets[id]->swap();
		}

		this->trieDefs[id] = new Trie();
		this->trieKeys[id] = new Trie();
		this->addToTrieDefs(id);
		this->addToTrieKeys(id);
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
			defText += "+ " +  ans.second + "\n";
		}

		return defText;

	}

	vector<string> getListOfWords(string prefix, int maximum) {
		//cerr << this->tries.size() << '\n';
		return this->trieKeys[this->curDataset]->getListOfWords(prefix, maximum);
	}

	vector<int> getRand(int tot) {
		return this->datasets[this->curDataset]->getRand(tot);
	}

	pair<string, string> getData(int id) {
		return this->datasets[this->curDataset]->getData(id);
	}
};

