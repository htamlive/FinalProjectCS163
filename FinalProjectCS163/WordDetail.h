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

	DataExecution* dataExec;
	vector<Trie*> tries;
	vector<vector<string>>* tmpDataSet;
	vector<int> IDs;
	string curString;
	bool hasChangeDef = false;
	bool favorite = false;

public:
	string getDefinition(string& str) {
		return this->dataExec->getDefinition(str);
	}

	void setOnClickEdit() {
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(true);
	}

	void setOnClickGreen() {
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setReadOnly(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(false);
	}

	WordDetail(tgui::Gui* GUI, int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h) {
		this->favorite = false;
		this->gui = GUI;
		this->dataExec = &DataExecution::getInstance();
		this->curString = str;
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
	
		this->IDs = this->dataExec->getID(this->curString);
		for (int i = 0; i < IDs.size(); i++) {
			this->favorite = this->favorite || this->dataExec->isFavorite(IDs[i]);
		}
	
		if (this->favorite) this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/bright_star.png");

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->onClick([&]() {
			if (!favorite) {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/bright_star.png");
				this->dataExec->addFavoriteIDs(this->IDs);
			}
			else {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");
				this->dataExec->removeFavoriteIDs(this->IDs);
			}
			
			favorite = !favorite;
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText(tgui::String(this->getDefinition(str)));
		if (this->curString.size() && this->curString[0] >= 'a' && this->curString[0] <= 'z') {
			this->curString[0] -= 32;
		}
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(this->curString));

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->onClick([&]() {
			this->setOnClickEdit();
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->onClick([&]() {
			if (!this->hasChangeDef) {
				cerr << "Nothing has changed\n";
				this->setOnClickGreen();
				return;
			}
			cout << "I've change yo mama\n";
			for (int i = 0; i < this->IDs.size(); i++) {
				this->dataExec->removeWord(this->IDs[i]);
			}
			this->dataExec->removeFavoriteIDs(this->IDs);

			string newDef = this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->getText().toStdString();
			if (newDef != "") this->dataExec->addWord(make_pair(this->curString, newDef));
			this->setOnClickGreen();
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->onTextChange([&]() {
			this->hasChangeDef = true;
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->onClick([&]() {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText("");
			this->hasChangeDef = true;
			});
	}

	void changeWord(const string& str) {
		static int childCount = 0;
		childCount++;
		this->favorite = false;
		this->hasChangeDef = false;
		this->curString = str;
		this->IDs = this->dataExec->getID(this->curString);
		for (int i = 0; i < IDs.size(); i++) {
			this->favorite = this->favorite || this->dataExec->isFavorite(IDs[i]);
		}

		if (this->favorite) this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/bright_star.png");
		else this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImage("images/dark_star.png");

		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(this->curString));
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->setText(tgui::String(this->getDefinition(this->curString)));

		if (!this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->isVisible()) {
			this->setOnClickGreen();
		}

		if (this->curString.size() && this->curString[0] >= 'a' && this->curString[0] <= 'z') {
			this->curString[0] -= 32;
		}
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("EditBox1")->setText(tgui::String(str));
	}
};