#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "DataExecution.h"
#include "WordDetail.h"

class FavoriteList
{
private:
	tgui::Gui* gui;
	int tot;
	bool isVisible;
	std::vector<int> memo;
	DataExecution* dataExec;
	WordDetail** wordDetail;


	void setup() {
		this->gui->get<tgui::ListView>("FavoriteList")->setVisible(false);
		this->gui->get<tgui::ListView>("FavoriteList")->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
		this->gui->get<tgui::ListView>("FavoriteList")->addColumn("Favorite List");

	}

	void initButtons() {
		auto btnFavor = tgui::ToggleButton::create();
		this->gui->add(btnFavor, "togBtnList");
		this->gui->get<tgui::ToggleButton>("togBtnList")->setPosition({930, 0});
		this->gui->get<tgui::ToggleButton>("togBtnList")->setSize({120, 56});
		this->gui->get<tgui::ToggleButton>("togBtnList")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("BtnFavorite"));
		this->gui->get<tgui::ToggleButton>("togBtnList")->onClick([&, this] {
			isVisible = !isVisible;
			this->gui->get<tgui::ListView>("FavoriteList")->setVisible(isVisible);
			this->gui->get<tgui::ToggleButton>("togBtnList")->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			this->memo = this->dataExec->getFavor();
			this->gui->get<tgui::ListView>("FavoriteList")->removeAllItems();
			for (int i = 0; i < this->memo.size(); ++i) {
				auto tmp = this->dataExec->getData(this->memo[i]);
				//cout << tmp.first << " " << tmp.second << "\n";

				this->gui->get<tgui::ListView>("FavoriteList")->addItem(reduceStr(tmp.first + ": " + tmp.second, 34));
			}
			});

		
	}

	void reload() {
		auto nwMemo = this->dataExec->getFavor();
		bool flag = nwMemo.size() == this->memo.size();
		
		for (int i = 0; flag && i < nwMemo.size(); ++i) {
			if (nwMemo[i] != memo[i]) flag = false;
		}
		if (flag) return;
		this->memo = nwMemo;
		this->gui->get<tgui::ListView>("FavoriteList")->removeAllItems();
		vector<int> filter;
		for (int i = 0; i < this->memo.size(); ++i) {
			auto tmp = this->dataExec->getData(this->memo[i]);
			if (tmp.first == "") continue;
			filter.push_back(memo[i]);
			//cout << tmp.first << " " << tmp.second << "\n";

			this->gui->get<tgui::ListView>("FavoriteList")->addItem(reduceStr(tmp.first + ": " + tmp.second, 34));
		}

		//this->dataExec->reloadFavor(-1,filter);
	}

	string reduceStr(string s, int l) {
		if (s.length() <= l + 3) return s;
		s = s.substr(0, l);
		s += "...";
		return s;
	}

	void setupWordDetail(int i) {
		string keys = this->dataExec->getData(this->memo[i]).first;
		if ((*this->wordDetail)) {
			(*this->wordDetail)->changeWord(keys);
		}
		else {
			*this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, keys);

		}
	}
public:
	FavoriteList(tgui::Gui* gui, WordDetail** wordDetail) {
		this->gui = gui;
		this->isVisible = false;
		this->dataExec = &DataExecution::getInstance();
		this->wordDetail = wordDetail;
		setup();
		initButtons();
	}

	void update() {
		
		int id = this->gui->get<tgui::ListView>("FavoriteList")->getSelectedItemIndex();
		if (id != -1) {
			setupWordDetail(id);
			this->gui->get<tgui::ListView>("FavoriteList")->deselectItems();
		} else reload();
	}
};

