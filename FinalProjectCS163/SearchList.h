#pragma once
#include <vector>
#include <string>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "WordDetail.h"
#include "FavoriteList.h"
class SearchList
{
private:
	vector<string> defOpts = {"btnWordSet", "btnWordSeq", "btnWordCons"};

	int x, y, w, h;
	std::vector<tgui::String> suggestedKeys;
	std::vector<int> suggestedIdx;
	tgui::Gui* gui;
	
	int curSet = 0;
	int curDefOpt = 0;
	bool* isWordModePtr, filterFavor = false;
	WordDetail* wordDetail;
	FavoriteList* favoriteList;
	DataExecution* dataExec;

	void showHistory();

	void updateDecoration();
	void updateDecoration(int cnt);

	tgui::String reduceStr(tgui::String s, int l);

	void setupWordDetail(int i);
	void setupWordDetail(int i, bool rand);
	
	void moveVerticalDefBtn(int id, int step);

	std::vector<tgui::String> getSuggestedKeys(tgui::String s);

	std::vector<int> getSuggestedIdx(tgui::String s);
	
public:

	SearchList(tgui::Gui* gui, const int& curSet, int x, int y, int w, int h, bool* isWordMode);

	void initSearchButton();

	void updateBtns();

	void onChangingText();

	void clear();

	void changeSearchSet(const int& curSet);

	bool checkSuggestion(tgui::String& str);

	void showDefSearchOptions(bool flag = true);

	tgui::String turnOneLine(tgui::String& need);

	void showSuggestions(std::vector<tgui::String> nwData = {}, std::vector<int> nwDataIdx = {});

	~SearchList();

	void update();
};

