#pragma once

#include <fstream>
#include <vector>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class FavoriteList
{
private:
	tgui::Gui* gui;
	int tot;
	bool isVisible;
	std::vector<int> memo;
	void loadFile() {
		std::ifstream ifs("Dataset/FavoriteId.txt");
		ifs >> this->tot;
		memo.resize(this->tot);
		for (int i = 0; i < this->tot; ++i) {
			ifs >> memo[i];
		}
		ifs.close();
	}

	void setup() {
		this->gui->get<tgui::ListView>("FavoriteList")->setVisible(false);
		this->gui->get<tgui::ListView>("FavoriteList")->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
		std::cout << this->gui->get<tgui::ListView>("FavoriteList")->addColumn("Favorite List");
		for (int i = 0; i < this->tot; ++i) {
			// load from dataset
		}
	}

	void initButtons() {
		auto btnFavor = tgui::ToggleButton::create();
		this->gui->add(btnFavor, "togBtnList");
		this->gui->get<tgui::ToggleButton>("togBtnList")->setPosition({1190, 0});
		this->gui->get<tgui::ToggleButton>("togBtnList")->setSize({120, 56});
		this->gui->get<tgui::ToggleButton>("togBtnList")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("BtnFavorite"));
		this->gui->get<tgui::ToggleButton>("togBtnList")->onClick([&, this] {
			isVisible = !isVisible;
			this->gui->get<tgui::ListView>("FavoriteList")->setVisible(isVisible);
			this->gui->get<tgui::ToggleButton>("togBtnList")->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			});

		
	}
public:
	FavoriteList(tgui::Gui* gui) {
		this->gui = gui;
		this->isVisible = false;
		loadFile();
		setup();
		initButtons();
	}

	void update() {

	}
};

