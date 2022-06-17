#pragma once
#include <vector>
#include <string>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>

class SearchList
{
private:
	int x, y, w, h;
	std::vector<std::string> data;
	tgui::Gui* gui;

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

	SearchList(tgui::Gui* gui,int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
		this->gui = gui;
		
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
			eb->setTextPosition({ "2%", "40%" },{0, 0});
			this->gui->add(eb);
		}
	};
};

