#include "SearchList.h"

void SearchList::showHistory() {
	const vector<int>& history = this->dataExec->getHistory(this->curSet);

	int mx = min(8, (int)history.size());
	vector<tgui::String> nwData;
	vector<int> nwDataIdx;
	vector<int> filter;
	for (int i = 0; i < mx; ++i) {
		tgui::String tmp = this->dataExec->getData(history[history.size() - 1 - i], this->curSet).first;
		if (tmp == "") continue;
		if (*this->isWordModePtr) nwData.push_back(tmp);
		else {
			nwDataIdx.push_back(history[history.size() - 1 - i]);
		}

		filter.push_back(history[history.size() - 1 - i]);
	}
	reverse(filter.begin(), filter.end());
	this->dataExec->reloadHistory(-1, filter);

	showSuggestions(nwData, nwDataIdx);

}

void SearchList::updateDecoration() {
	this->gui->get<tgui::Picture>("BottomList")->setVisible(false);
	this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(false);
}

void SearchList::updateDecoration(int cnt) {
	if (cnt) {
		this->gui->get<tgui::Picture>("BottomList")->setVisible(true);
		this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(true);
		this->gui->get<tgui::Picture>("BottomList")->setPosition(this->x, this->y + cnt * this->h - 5);
	} else {
		this->gui->get<tgui::Picture>("BottomList")->setVisible(false);
		this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(false);
	}
}

tgui::String SearchList::reduceStr(tgui::String s, int l) {
	if (s.length() <= l + 3) return s;
	s = s.substr(0, l);
	s += "...";
	return s;
}

void SearchList::setupWordDetail(int i) {
	if (this->wordDetail) {
		if (*this->isWordModePtr == true)
			this->wordDetail->changeWord(suggestedKeys[i]);
		else {
			tgui::String str = this->dataExec->getData(suggestedIdx[i]).first;
			this->wordDetail->changeWord(str);
		}
	}
	else {
		if (*this->isWordModePtr == true)
			this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, suggestedKeys[i]);
		else {
			tgui::String str = this->dataExec->getData(suggestedIdx[i]).first;
			this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, str);
		}
	}
}

void SearchList::setupWordDetail(int i, bool rand) {
	if (this->wordDetail) {
		if (rand) {
			this->wordDetail->changeWord(this->dataExec->getData(i).first);
		}
		else if (*this->isWordModePtr == true)
			this->wordDetail->changeWord(suggestedKeys[i]);
		else {
			tgui::String str = this->dataExec->getData(suggestedIdx[i]).first;
			this->wordDetail->changeWord(str);
		}
	} else {
		if (rand) {
			this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, this->dataExec->getData(i).first);
		}
		else if (*this->isWordModePtr == true)
			this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, suggestedKeys[i]);
		else {
			tgui::String str = this->dataExec->getData(suggestedIdx[i]).first;
			this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, str);
		}
	}
}

void SearchList::moveVerticalDefBtn(int id, int step) {
	auto pos = this->gui->get<tgui::Button>(defOpts[id])->getPosition();
	this->gui->get<tgui::Button>(defOpts[id])->moveWithAnimation({ pos.x, pos.y + step }, 100);
}

std::vector<tgui::String> SearchList::getSuggestedKeys(tgui::String s) {
	s = s.toLower();
	vector<tgui::String> result;
	result = this->dataExec->getListOfKeys(s, -1);
	if (this->filterFavor) {
		result = this->dataExec->filterFavor(result);
	}
	if (result.size() >= 8)  result.resize(8);
	return result;
}

std::vector<int> SearchList::getSuggestedIdx(tgui::String s) {
	s = s.toLower();
	vector<int> nwDataId, result;
	switch (this->curDefOpt)
	{
	case 0:
		nwDataId = this->dataExec->getKeys(s, INT32_MAX);
		break;
	case 1:
		nwDataId = this->dataExec->getKeysSubsequence(s, INT32_MAX);
		break;
	case 2:
		nwDataId = this->dataExec->getKeysSubarray(s, INT32_MAX);
		break;
	default:
		break;
	}

	if (this->filterFavor) {
		result = this->dataExec->filterFavor(nwDataId);
	}
	else result = nwDataId;
	if (result.size() >= 8)  result.resize(8);
	return result;
}

SearchList::SearchList(tgui::Gui* gui, const int& curSet, int x, int y, int w, int h, bool* isWordMode) : x(x), y(y), w(w), h(h), curSet(curSet) {
	this->gui = gui;
	this->isWordModePtr = isWordMode;
	this->wordDetail = nullptr;
	this->dataExec = &DataExecution::getInstance();
	this->favoriteList = new FavoriteList(this->gui, &wordDetail, &filterFavor);

	this->initSearchButton();
	this->showDefSearchOptions(false);
	moveVerticalDefBtn(this->curDefOpt, -15);

	this->gui->get<tgui::Button>("btnHistory")->onClick([&, this] {
		if (this->curSet != this->dataExec->getCurDataset()) {
			this->curSet = this->dataExec->getCurDataset();
		}
		showHistory();
		});

	//showHistory();
	this->gui->get<tgui::EditBox>("SearchBar")->onTextChange([&, this]() {
		if (*this->isWordModePtr == true) onChangingText();
		else {
			int cnt = this->gui->get<tgui::EditBox>("SearchBar")->getText().size();
			if (cnt > 0) this->clear();
		}
		});


	this->gui->get<tgui::EditBox>("SearchBar")->onFocus([&, this]() {
		onChangingText();
		});
	//this->wordDetail->setVisible(false);
};

void SearchList::initSearchButton() {
	this->gui->get<tgui::Button>("btnSearch")->onClick([&, this]() {
		tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
		if (text.length() == 0) {
			if (!this->filterFavor)
				this->setupWordDetail(this->dataExec->getRand(1)[0], true);
			return;
		}
		if (!checkValidString(text)) {
			this->gui->get<tgui::EditBox>("SearchBar")->setText("");
			return;
		}
		text = text.toLower();
		if (*this->isWordModePtr == true) {
			if (this->filterFavor && (this->suggestedKeys.empty() || !this->checkSuggestion(text))) {
				return;
			};
			if (this->wordDetail) this->wordDetail->changeWord(text);
			else this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, text);
		}
		else {
			if (!this->dataExec->isUnicode) turnNonUnicodeString(text);
			showSuggestions({}, this->getSuggestedIdx(text));
		}
		});
}

void SearchList::updateBtns() {
	for (int i = 0; i < this->defOpts.size(); ++i) {
		if (this->gui->get<tgui::Button>(defOpts[i])->isFocused() && i != this->curDefOpt) {
			this->gui->get<tgui::Button>(defOpts[this->curDefOpt])->leftMouseButtonNoLongerDown();
			moveVerticalDefBtn(this->curDefOpt, 15);
			this->curDefOpt = i;
			clear();
			this->gui->get<tgui::Button>(defOpts[i])->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			moveVerticalDefBtn(this->curDefOpt, -15);
		}
	}
	this->gui->get<tgui::Button>(this->defOpts[this->curDefOpt])->leftMousePressed({});
}

void SearchList::onChangingText() {
	tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
	if (!checkValidString(text)) {
		clear();
		return;
	}
	tgui::String stdText = text;
	if (!this->dataExec->isUnicode) {
		stdText.clear();
		for (int i = 0; i < (int)text.length(); i++) {
			stdText.push_back(atomic_char32_t(text[i]));
		}
		bool check = turnNonUnicodeString(stdText);
		stdText = stdText.toLower();
	}

	if (stdText.length() < 1) {
		if (!this->filterFavor) this->showHistory();
		else clear();
	}
	else {
		vector<tgui::String> nwData;
		if (*this->isWordModePtr) {
			nwData = this->getSuggestedKeys(stdText);
		}
		showSuggestions(nwData);
	}
}

void SearchList::clear() {
	for (int i = 0; i < suggestedKeys.size(); ++i) {
		this->gui->remove(this->gui->get<tgui::Button>("op" + std::to_string(i)));
	}

	for (int i = 0; i < suggestedIdx.size(); ++i) {
		this->gui->remove(this->gui->get<tgui::Button>("op" + std::to_string(i)));
	}

	this->suggestedKeys.clear();
	this->suggestedIdx.clear();
	this->updateDecoration();
}

void SearchList::changeSearchSet(const int& curSet) {
	this->curSet = curSet;
}

bool SearchList::checkSuggestion(tgui::String& str) {
	tgui::String getDef = this->dataExec->getDefinition(str);
	return getDef != "";

}

void SearchList::showDefSearchOptions(bool flag) {
	for (auto x : this->defOpts) {
		this->gui->get<tgui::Button>(x)->setVisible(flag);
	}
}

tgui::String SearchList::turnOneLine(tgui::String& need) {
	tgui::String display = "";

	for (int j = 0; j < (int)need.size(); j++) {
		if (need[j] == '\n') {
			if (!display.empty() && display.back() != ' ') {
				display += ' ';
			}
			continue;
		}
		display += need[j];
	}
	return display;
}

void SearchList::showSuggestions(std::vector<tgui::String> nwData, std::vector<int> nwDataIdx) {
	clear();
	this->suggestedKeys = nwData;
	this->suggestedIdx = nwDataIdx;
	int cnt = 0;
	for (int i = 0; i < suggestedKeys.size() + suggestedIdx.size(); ++i) {
		if (*this->isWordModePtr && !this->checkSuggestion(suggestedKeys[i])) continue;

		auto eb = tgui::Button::create();
		eb->setWidgetName("op" + std::to_string(i));
		eb->setPosition(x, y + cnt * this->h);
		eb->setSize(w, h);

		if (*this->isWordModePtr == true) eb->setText(suggestedKeys[i]);
		else {
			auto tmp = this->dataExec->getData(nwDataIdx[i]);
			eb->setText(this->reduceStr(tmp.first + ": " + turnOneLine(tmp.second), 80));
		}
		eb->setTextSize(16);
		eb->setTextPosition({ "2%", "40%" }, { 0, 0 });

		eb->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordButton"));
		this->gui->add(eb);
		eb->onClick([i, this]() {
			setupWordDetail(i);
			this->onChangingText();
			});
		cnt++;
	}

	this->updateDecoration(cnt);

};

SearchList::~SearchList() {
	if (this->wordDetail) {
		delete this->wordDetail;
	}
	if (this->favoriteList) {
		delete this->favoriteList;
	}

}

void SearchList::update() {
	if (this->gui->getFocusedChild() == nullptr || this->gui->getFocusedChild() == this->gui->get<tgui::ToggleButton>("togFilterStars"))
		showSuggestions({});
	this->favoriteList->update();
	if (this->wordDetail != NULL) {
		this->wordDetail->update();
	}
	this->updateBtns();
}