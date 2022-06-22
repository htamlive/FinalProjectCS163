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

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
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

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText(tgui::String(this->getDefinition(str)));
		if (str.size() && str[0] >= 'a' && str[0] <= 'z') {
			str[0] -= 32;
		}
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(str));

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->onClick([&]() {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(false);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(false);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(true);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(true);
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->onClick([&]() {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(true);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(true);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(false);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(false);
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->onClick([&]() {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText("");
			});
	}

	void changeWord(int& curSet, string& str) {
		static int childCount = 0;
		childCount++;
		this->favorite = false;
		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(str));
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText(tgui::String(this->getDefinition(str)));

		if (!this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->isVisible()) {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(false);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(false);
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(true);
		}

		if (str.size() && str[0] >= 'a' && str[0] <= 'z') {
			str[0] -= 32;
		}
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(str));

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(true);
	}
};