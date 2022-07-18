#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "DataExecution.h"
#include "WordDetail.h"

class FavoriteList {
private:
	tgui::Gui* gui;
	int tot;
	bool isVisible, *FavorFilterPtr;
	std::vector<int> memo;
	DataExecution* dataExec;
	WordDetail** wordDetail;


	void setup();

	void initBtnFavorList();

	void initBtnFilterStars();

	void initButtons();

	void reload();

	void setupWordDetail(int i);

public:
	FavoriteList(tgui::Gui* gui, WordDetail** wordDetail, bool* wordFilterPtr);

	void update();
};

