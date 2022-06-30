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
	vector<int> IDs;
	string curString;
	bool hasChangeDef = false;
	bool onChangeDef = false;
	bool favorite = false;


public:
	string getDefinition(string& str) {
		return this->dataExec->getDefinition(str);
	}

	void setOnClickEdit() {
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setReadOnly(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(true);
	}

	void setOnClickGreen() {
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setReadOnly(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("GreenButton")->setVisible(false);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->setVisible(false);
	}

	void initFavorBtn() {
		if (this->favorite) this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/bright_star.png");

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->onClick([&, this]() {
			if (!favorite) {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/bright_star.png");
				this->dataExec->addFavoriteIDs(this->IDs);
			}
			else {
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/dark_star.png");
				this->dataExec->removeFavoriteIDs(this->IDs);
			}

			favorite = !favorite;
			});

		if (this->favorite) this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/bright_star.png");
		else this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/dark_star.png");
	}

	void initAcceptedBtn() {
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
				this->dataExec->removeHistoryIDs(this->IDs);

				tgui::String stdText = this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->getText().toStdString();
				tgui::String newDef;

				for (int i = 0; i < (int)stdText.length(); i++) {
				newDef.push_back(atomic_char32_t(stdText[i]));
				}
				bool check = turnNonUnicodeString(newDef);
				newDef = newDef.toStdString();
				if (newDef != "") {
					this->dataExec->addWord(make_pair(this->curString, newDef.toStdString()));
				}
				this->setOnClickGreen();
			});
	}

	void initDefinitionArea() {
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("ebKey")->setInheritedFont(tgui::Font("Template\\fonts\\UTM AvoBold_Italic.ttf"));

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setText(tgui::String(this->getDefinition(curString)));
		if (this->curString.size() && this->curString[0] >= 'a' && this->curString[0] <= 'z') {
			this->curString[0] -= 32;
		}

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->onTextChange([&]() {
			this->hasChangeDef = true;
			});
	}

	WordDetail(tgui::Gui* GUI, int x, int y, int w, int h, string str) : x(x), y(y), w(w), h(h) {
		this->favorite = false;
		this->gui = GUI;
		this->dataExec = &DataExecution::getInstance();
		this->curString = str;
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("btnFavor")->setImage("images/dark_star.png");
		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		
		this->IDs = this->dataExec->getID(this->curString);
		if (this->IDs.size()) {
			this->dataExec->addHistoryID(this->IDs[0]);
		}

		for (int i = 0; i < IDs.size(); i++) {
			this->favorite = this->favorite || this->dataExec->isFavorite(IDs[i]);
		}

		this->initFavorBtn();

		this->initDefinitionArea();



		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("RedButton")->onClick([&]() {
			this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setText("");
			this->hasChangeDef = true;
			this->onChangeDef = true;
			});

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("ebKey")->setText(tgui::String(this->curString));

		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->onClick([&]() {
			this->setOnClickEdit();
			});

		this->initAcceptedBtn();
		//this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("TextArea1")->

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



		this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("ebKey")->setText(tgui::String(this->curString));
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setText(tgui::String(this->getDefinition(this->curString)));

		if (!this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::Button>("EditButton")->isVisible()) {
			this->setOnClickGreen();
		}

		if (this->curString.size() && this->curString[0] >= 'a' && this->curString[0] <= 'z') {
			this->curString[0] -= 32;
		}
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::EditBox>("ebKey")->setText(tgui::String(str));
	}

	void update() {
		if (this->onChangeDef) {
			this->onChangeDef = false;
			tgui::String txt = this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->getText();
			if(!checkValidString(txt)){
				this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::TextArea>("txtDef")->setText("");
			}
		}
	}
};