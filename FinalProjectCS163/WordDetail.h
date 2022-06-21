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
	vector<vector<string>>* tmpDataSet;
	bool favorite = false;

public:
	string getDefinition(string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		vector<pair<int, int>> def = this->tries[this->curSet]->getDefinitions(str);
		string defText = "";
		for (int i = 0; i < (int)def.size(); i++) {
			defText += (*tmpDataSet)[def[i].first][def[i].second];
			defText += ' ';
		}

		return defText;
	}

	WordDetail(tgui::Gui* GUI, int& curSet, vector<Trie*> tries, vector<vector<string>>* tmpData, int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h), curSet(curSet) {
		
		this->gui = GUI;
		static int childCount = 0;
		childCount++;
		this->tries = tries;
		this->tmpDataSet = tmpData;

		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->onClick([&]() {
			if (!favorite) {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/bright_star.png");
			}
			else {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
			}
			favorite = !favorite;
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(str));
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText(tgui::String(this->getDefinition(str)));
	}
};