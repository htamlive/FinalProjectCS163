#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "DataExecution.h"

using namespace tgui;
using namespace std;

class WordDetail
{
private:
	int x, y, w, h;
	tgui::Gui* gui;
	int curSet;

	DataExecution* dataExec;
	vector<Trie*> tries;
	vector<vector<string>>* tmpDataSet;
	bool favorite = false;

public:
	string getDefinition(string& str) {
		return this->dataExec->getDefinition(str);
	}


	WordDetail(tgui::Gui* GUI, int& curSet,int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h), curSet(curSet) {

		this->gui = GUI;
		this->dataExec = &DataExecution::getInstance();
		static int childCount = 0;
		childCount++;
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
		//cerr << "It's here\n";

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

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("Button2")->onClick([&]() {
			bool check = this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->isReadOnly();
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(!check);
			});
	}
};