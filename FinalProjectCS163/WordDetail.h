#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "DataExecution.h"

using namespace tgui;
using namespace std;

class WordDetail {
private:
	int x, y, w, h;
	tgui::Gui* gui;

	DataExecution* dataExec;
	vector<int> IDs;
	tgui::String curString;
	bool hasChangeDef = false;
	bool onChangeDef = false;
	bool favorite = false;

public:
	tgui::String getDefinition(tgui::String& str);

	void setOnClickEdit();

	void setOnClickGreen();

	void initFavorBtn();

	void initAcceptedBtn();

	void initInputArea();

	void initTrashCan();

	WordDetail(tgui::Gui* GUI, int x, int y, int w, int h, tgui::String str);

	void changeWord(const tgui::String& str);

	void update();
};