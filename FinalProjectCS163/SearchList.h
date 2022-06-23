#pragma once
#include <vector>
#include <string>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "WordDetail.h"
class SearchList
{
private:
	int x, y, w, h;
	std::vector<std::string> data;
	tgui::Gui* gui;
	
	int curSet = 0;
	vector<Trie*> tries;
	WordDetail* wordDetail;
	vector<vector<string>>* tmpDataSet;
	vector<int> history;
	DataExecution* dataExec;

	//sf::Texture backgroundTexture;
	//RectangleShape background;

	void showHistory() {
		vector<pair<string, string>> tmp(this->history.size());
		vector<string> nwData;
		for (int i = 0; i < this->history.size(); ++i) {
			tmp[i] = this->dataExec->getData(this->history[i],this->curSet);
			nwData.push_back(tmp[i].first);
		}
		showSuggestions(nwData);
		
	}

public:
	SearchList() {
		
	};

	SearchList(tgui::Gui* gui, const int& curSet, vector<Trie*> tries, vector<vector<string>>* tmpData, int x, int y, int w, int h) : x(x), y(y), w(w), h(h), curSet(curSet) {
		this->gui = gui;
		this->tries = tries;
		this->tmpDataSet = tmpData;
		this->dataExec = &DataExecution::getInstance();
		this->history = this->dataExec->loadHistory(this->curSet);

		this->gui->get<tgui::Button>("btnHistory")->onClick([&, this] {
			if (this->curSet != this->dataExec->getCurDataset()) {
				this->curSet = this->dataExec->getCurDataset();
				this->history = this->dataExec->loadHistory(this->curSet);
			}
			showHistory();
			});

		//showHistory();
		this->gui->get<tgui::EditBox>("SearchBar")->onTextChange([&]() {
			onChangingText();
		});


		this->gui->get<tgui::EditBox>("SearchBar")->onFocus([&]() {
			onChangingText();
			});
		//this->wordDetail->setVisible(false);
	};

	void onChangingText() {
		tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
		text = text.toLower();
		if (text.length() < 1) {
			//cerr << "Type down more shit you idiot\n";
			this->showHistory();
		}
		else {
			auto nwData = this->dataExec->getListOfWords(text.toStdString(), 8);
			showSuggestions(nwData);
		}
		

	}

	void clear() {
		for (int i = 0; i < data.size(); ++i) {
			this->gui->remove(this->gui->get<tgui::Button>("op" + std::to_string(i)));
		}
	}

	void changeSearchSet(const int& curSet) {
		this->curSet = curSet;
	}

	void showSuggestions(std::vector<std::string> nwData) {
		clear();
		this->data = nwData;
		for (int i = 0; i < data.size(); ++i) {
			auto eb = tgui::Button::create();
			eb->setWidgetName("op" + std::to_string(i));
			eb->setPosition(x, y + i * this->h);
			eb->setSize(w, h);
			eb->setText(data[i]);
			eb->setTextSize(16);
			eb->setTextPosition({ "2%", "40%" },{0, 0});
			
			//eb->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(50));
			eb->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordButton"));
			this->gui->add(eb);
			eb->onClick([i, this]() {
				if (this->wordDetail) {
					this->wordDetail->changeWord(curSet, data[i]);
				}
				else {
					this->wordDetail = new WordDetail(this->gui, this->curSet, 25, 100, 450, 600, data[i]);
					
					//std::cout << data[i] << "\n";
				}
				});
		}
		
		if (data.size() > 0) {
			this->gui->get<tgui::Picture>("BottomList")->setVisible(true);
			this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(true);
			this->gui->get<tgui::Picture>("BottomList")->setPosition(this->x, this->y + data.size() * this->h - 5);
		}
		else {
			
			this->gui->get<tgui::Picture>("BottomList")->setVisible(false);
			this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(false);
		}
		
	};

	void update() {
		//std::cout << x->getWidgetName() << "\n";
		if (this->gui->getFocusedChild() == nullptr)
			showSuggestions({});

	}
};

