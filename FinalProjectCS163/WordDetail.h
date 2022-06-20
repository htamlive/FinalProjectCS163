#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Trie.h"

using namespace tgui;
using namespace std;

class WordDetail
{
private:
	int x, y, w, h;
	tgui::Gui* gui;
	int curSet;
	vector<Trie*> tries;
	vector<vector<string>> tmpDataSet;

public:
	bool on = false;
	WordDetail(tgui::Gui* GUI, int& curSet, vector<Trie*> tries, vector<vector<string>>& tmpData, int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h), curSet(curSet) {
		this->gui = GUI;
		this->tries = tries;
		this->tmpDataSet = tmpData;
		auto chWindow = tgui::ChildWindow::create();
		chWindow->setWidgetName("chWindow");
		chWindow->setPosition(x, y);
		chWindow->setSize(w, h);

		auto editBoxK = tgui::EditBox::create();
		editBoxK->setWidgetName("editBoxKeyWord");
		editBoxK->setPosition(150, 300);
		editBoxK->setSize(150, 30);
		editBoxK->setTextSize(14);
		editBoxK->setReadOnly();
		editBoxK->setAlignment(tgui::EditBox::Alignment::Center);
		editBoxK->setText(tgui::String(str));
		chWindow->add(editBoxK);

		auto editBoxD = tgui::TextArea::create();
		editBoxD->setWidgetName("editBoxDefinition");
		editBoxD->setPosition(75, 350);
		editBoxD->setSize(300, 200);
		editBoxD->setTextSize(14);
		//editBoxD->limitTextWidth();
		//editBoxD->setReadOnly();

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		vector<pair<int, int>> def = this->tries[this->curSet]->getDefinitions(str);
		string defText = "";
		for (int i = 0; i < (int)def.size(); i++) {
			defText += tmpDataSet[def[i].first][def[i].second];
			defText += ' ';
		}

		editBoxD->setDefaultText("Definition goes here");
		editBoxD->setText(tgui::String(defText));
		chWindow->add(editBoxD);
		this->gui->add(chWindow);

		on = true;
	}

	void removeWindow() {
		this->gui->get<tgui::ChildWindow>("chWindow")->close();
		this->on = false;
	}

	void setVisible(bool isVisible) {
		this->gui->get<tgui::ChildWindow>("chWindow")->setVisible(isVisible);
	}
};