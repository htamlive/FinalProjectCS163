#include "FavoriteList.h"

void FavoriteList::setup() {
	this->gui->get<tgui::ListView>("FavoriteList")->setVisible(false);
	this->gui->get<tgui::ListView>("FavoriteList")->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
	this->gui->get<tgui::ListView>("FavoriteList")->addColumn("Favorite List");

}

void FavoriteList::initBtnFavorList() {
	auto btnFavor = tgui::ToggleButton::create();
	this->gui->add(btnFavor, "togBtnList");
	this->gui->get<tgui::ToggleButton>("togBtnList")->setPosition({ 930, 0 });
	this->gui->get<tgui::ToggleButton>("togBtnList")->setSize({ 120, 56 });
	this->gui->get<tgui::ToggleButton>("togBtnList")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("BtnFavorite"));
	this->gui->get<tgui::ToggleButton>("togBtnList")->onClick([&, this] {
		isVisible = !isVisible;
		this->gui->get<tgui::ToggleButton>("togFilterStars")->setVisible(isVisible);
		this->gui->get<tgui::ListView>("FavoriteList")->setVisible(isVisible);
		if (!isVisible) {
			this->gui->get<tgui::ToggleButton>("togFilterStars")->setDown(false);
			*this->FavorFilterPtr = false;
		}
		this->gui->get<tgui::ToggleButton>("togBtnList")->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
		this->memo = this->dataExec->getFavor();
		this->gui->get<tgui::ListView>("FavoriteList")->removeAllItems();
		for (int i = 0; i < this->memo.size(); ++i) {
			auto tmp = this->dataExec->getData(this->memo[i]);

			this->gui->get<tgui::ListView>("FavoriteList")->addItem(reduceUnicodeStr(tmp.first.toLower() + ": " + tmp.second, 34));
		}
		});
}

void FavoriteList::initBtnFilterStars() {
	auto btnFavor = tgui::ToggleButton::create();
	this->gui->add(btnFavor, "togFilterStars");
	this->gui->get<tgui::ToggleButton>("togFilterStars")->setVisible(false);
	this->gui->get<tgui::ToggleButton>("togFilterStars")->setPosition({ 565, 120 });
	this->gui->get<tgui::ToggleButton>("togFilterStars")->setSize({ 92, 40 });
	this->gui->get<tgui::ToggleButton>("togFilterStars")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("BtnFilterStars"));
	this->gui->get<tgui::ToggleButton>("togFilterStars")->onClick([&, this] {
		*this->FavorFilterPtr = !*this->FavorFilterPtr;
		});
}

void FavoriteList::initButtons() {
	this->initBtnFilterStars();
	this->initBtnFavorList();

}

void FavoriteList::reload() {
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

		this->gui->get<tgui::ListView>("FavoriteList")->addItem(reduceUnicodeStr(tmp.first + ": " + tmp.second, 34));
	}

	this->dataExec->reloadFavor(-1, filter);
}

void FavoriteList::setupWordDetail(int i) {
	tgui::String keys = this->dataExec->getData(this->memo[i]).first;
	if ((*this->wordDetail)) {
		(*this->wordDetail)->changeWord(keys);
	}
	else {
		*this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, keys);
	}
}

FavoriteList::FavoriteList(tgui::Gui* gui, WordDetail** wordDetail, bool* wordFilterPtr) {
	this->gui = gui;
	this->isVisible = false;
	this->FavorFilterPtr = wordFilterPtr;
	this->dataExec = &DataExecution::getInstance();
	this->wordDetail = wordDetail;
	setup();
	initButtons();
}

void FavoriteList::update() {
	int id = this->gui->get<tgui::ListView>("FavoriteList")->getSelectedItemIndex();
	if (id != -1) {
		setupWordDetail(id);
		this->gui->get<tgui::ListView>("FavoriteList")->deselectItems();
	}
	else reload();
}