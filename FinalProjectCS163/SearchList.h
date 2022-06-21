#pragma once
#include <vector>
#include <string>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "WordDetail.h"
#include "Trie.h"

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

	//sf::Texture backgroundTexture;
	//RectangleShape background;

	void initBackground(int w, int h) {
		//this->background.setSize(sf::Vector2f(
		//	w, h
		//)
		//);
		//this->backgroundTexture.loadFromFile("images/whiteRec.png");
		//this->background.setTexture(&this->backgroundTexture);
	}

public:
	SearchList() {
		
	};

	SearchList(tgui::Gui* gui, int& curSet, vector<Trie*> tries, vector<vector<string>>* tmpData, int x, int y, int w, int h) : x(x), y(y), w(w), h(h), curSet(curSet) {
		this->gui = gui;
		this->tries = tries;
		this->tmpDataSet = tmpData;
		//this->wordDetail->setVisible(false);
	};

	void updateData(const std::vector<std::string>& data) {
		this->data = data;
	}

	void clear() {
		for (int i = 0; i < data.size(); ++i) {
			this->gui->remove(this->gui->get<tgui::Button>("op" + std::to_string(i)));
		}
	}

	void update(std::vector<std::string> nwData) {
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
					//this->wordDetail->removeWindow();
					//delete this->wordDetail;
					//this->wordDetail = nullptr;
				}

				this->wordDetail = new WordDetail(this->gui, this->curSet, this->tries, this->tmpDataSet, 25, 100, 450, 600, data[i]);
				//std::cout << data[i] << "\n";
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
};

