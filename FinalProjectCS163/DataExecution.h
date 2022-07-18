#pragma once
#include <vector>
#include "Trie.h"
#include "helper.h"
#include "UnicodeDATASET.h"
#include "UnicodeTrie.h"
#include <SFML/Network/Packet.hpp>

class DataExecution {
private:
	UnicodeDATASET* datasets[5];
	UnicodeTrie* trieKeys[5], * trieDefs[5];
	std::vector<int> favor[5];
	std::vector<int> history[5];

	bool finishDataset[5], finishKeys[5], finishDefs[5], isShutDown, isReload;

	bool loadSer = true;

	int curDataset;

	void addToTrieDefs(int id, bool isReload = false);

	void addToTrieKeys(int id, bool isReload = false);

	void loadFavor(int id);

	void saveFavor(int id);

	void loadHistory(int id);

	void saveHistory(int id);

public:
	const bool isUnicode = true;

	static DataExecution& getInstance();

	DataExecution();

	virtual ~DataExecution();

	void saveAndRemoveTrie(int id, string additionStr);

	bool loadKeys(int id, bool isReload = false, bool setCur = false);

	bool loadDefs(int id, bool isReload = false, bool setCur = false);

	void addWord(pair< tgui::String, tgui::String> word);

	bool loadDataset(int id, bool setCur = false);

	const vector<int>& getHistory(int type = -1);

	void clearHistory(int type = -1);

	bool isFavorite(int id);

	int getCurSet();

	tgui::String getDefinition(tgui::String& str);

	vector<int> getID(tgui::String& str);

	void addFavoriteIDs(vector<int>& IDs);

	void addHistoryID(int ID);

	void removeFavoriteIDs(vector<int> IDs);

	void removeHistoryIDs(vector<int> IDs);

	vector<int> getFavor(int id = -1);

	void reloadFavor(int id, const vector<int>& x);

	void reloadHistory(int id, const vector<int>& x);

	void setShutDown();

	bool getShutDown();

	void setReload(bool flag);

	bool getReload();

	bool checkFinishAll();

	vector<tgui::String> getListOfKeys(tgui::String prefix, int maximum);

	vector<int> getRand(int tot);

	pair<tgui::String, tgui::String> getData(int id, int curSet = -1);

	int getCurDataset();

	void restore(int id = -1);

	vector<int> getKeys(tgui::String s, int maximum = 8);

	vector<int> getKeysSubarray(tgui::String s, int maximum = 8);

	vector<int> getKeysSubsequence(tgui::String s, int maximum = 8);

	void removeWord(int id);

	vector<tgui::String> filterFavor(const vector<tgui::String>& suggestedKeys);

	vector<int> filterFavor(const vector<int>& suggestedIdx);
};

