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
	bool favorite = false;

public:
	bool on = false;
	WordDetail(tgui::Gui* GUI, int& curSet, vector<Trie*> tries, vector<vector<string>>& tmpData, int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h), curSet(curSet) {
		Theme theme{ "Template/themes/BabyBlue.txt" };

		static int childCount = 0;
		childCount++;
		this->gui = GUI;
		this->tries = tries;
		this->tmpDataSet = tmpData;

		//auto texture = tgui::Texture::Texture("images/teacher.jpg");
		auto picture = tgui::Picture::create("images/teacher.jpg");
		picture->setSize(450, 600);

		auto chWindow = tgui::ChildWindow::create();
		chWindow->setWidgetName(tgui::String("chWindow") + tgui::String(childCount));
		chWindow->setPosition(x, y);
		chWindow->setSize(w, h);
		chWindow->setRenderer(theme.getRenderer("ChildWindow"));
		chWindow->add(picture);
		
		auto editBoxK = tgui::EditBox::create();
		editBoxK->setRenderer(theme.getRenderer("EditBox"));
		editBoxK->setWidgetName("editBoxKeyWord");
		editBoxK->setPosition(150, 300);
		editBoxK->setSize(150, 30);
		editBoxK->setTextSize(16);
		editBoxK->setReadOnly();
		editBoxK->setAlignment(tgui::EditBox::Alignment::Center);
		editBoxK->setText(tgui::String(str));
		chWindow->add(editBoxK);

		auto editBoxD = tgui::TextArea::create();
		editBoxD->setRenderer(theme.getRenderer("TextArea"));
		editBoxD->setWidgetName("editBoxDefinition");
		editBoxD->setPosition(75, 350);
		editBoxD->setSize(300, 200);
		editBoxD->setTextSize(18);
		//editBoxD->limitTextWidth();
		editBoxD->setReadOnly();
		editBoxD->setDefaultText("Definition goes here");

		auto editButton = tgui::BitmapButton::create();
		editButton->setWidgetName("editButton");
		editButton->setPosition(320, 300);
		editButton->setSize(30, 30);
		editButton->setImage("images/pencil.png");
		editButton->setImageScaling(0.8f);
		editButton->onClick([&]() {
			bool check = this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::TextArea>("editBoxDefinition")->isReadOnly();
			this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::TextArea>("editBoxDefinition")->setReadOnly(!check);
			});
		chWindow->add(editButton);

		auto favoriteButton = tgui::BitmapButton::create();
		favoriteButton->setWidgetName("favoriteButton");
		favoriteButton->setPosition(25, 50);
		favoriteButton->setSize(50, 50);
		favoriteButton->setImage("images/dark_star.png");
		favoriteButton->setImageScaling(0.8f);
		favoriteButton->onMouseEnter([&]() {
			if (favorite) { return; }
			this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::BitmapButton>("favoriteButton")->setImage("images/bright_star.png");
			});
		favoriteButton->onMouseLeave([&]() {
			if (favorite) { return; }
			this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::BitmapButton>("favoriteButton")->setImage("images/dark_star.png");
			});
		favoriteButton->onClick([&]() {
			if (!favorite) {
				this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::BitmapButton>("favoriteButton")->setImage("images/bright_star.png");
			}
			else {
				this->gui->get<tgui::ChildWindow>(tgui::String("chWindow") + tgui::String(childCount))->get<tgui::BitmapButton>("favoriteButton")->setImage("images/dark_star.png");
			}
			favorite = !favorite;
			});
		chWindow->add(favoriteButton);

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		vector<pair<int, int>> def = this->tries[this->curSet]->getDefinitions(str);
		string defText = "";
		for (int i = 0; i < (int)def.size(); i++) {
			defText += tmpDataSet[def[i].first][def[i].second];
			defText += ' ';
		}
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