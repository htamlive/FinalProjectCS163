#pragma once
#include <vector>
#include <string>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "WordDetail.h"
#include "FavoriteList.h"
class SearchList
{
private:
	int x, y, w, h;
	std::vector<std::string> suggestedKeys;
	std::vector<int> suggestedIdx;
	tgui::Gui* gui;
	
	int curSet = 0;
	bool* isWordModePtr;
	WordDetail* wordDetail;
	FavoriteList* favoriteList;
	DataExecution* dataExec;

	void showHistory() {
		const vector<int>& history = this->dataExec->getHistory(this->curSet);
		
		int mx = min(8, (int)history.size());
		vector<string> nwData(mx);
		for (int i = 0; i < mx; ++i) {
			string tmp = this->dataExec->getData(history[history.size() - 1 - i],this->curSet).first;
			
			nwData[i] = tmp;
		}
		showSuggestions(nwData);
		
	}

	void updateDecoration(int cnt = 0) {
		if (cnt) {
			this->gui->get<tgui::Picture>("BottomList")->setVisible(true);
			this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(true);
			this->gui->get<tgui::Picture>("BottomList")->setPosition(this->x, this->y + cnt * this->h - 5);
		}
		else {

			this->gui->get<tgui::Picture>("BottomList")->setVisible(false);
			this->gui->get<tgui::Picture>("SearchDecoration")->setVisible(false);
		}
	}

	string reduceStr(string s, int l) {
		if (s.length() <= l + 3) return s;
		s = s.substr(0, l);
		s += "...";
		return s;
	}

	void setupWordDetail(int i) {
		if (this->wordDetail) {
			if (*this->isWordModePtr == true)
				this->wordDetail->changeWord(suggestedKeys[i]);
			else {
				string str = this->dataExec->getData(suggestedIdx[i]).first;
				this->wordDetail->changeWord(str);
			}
		}
		else {
			if (*this->isWordModePtr == true)
				this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, suggestedKeys[i]);
			else {
				string str = this->dataExec->getData(suggestedIdx[i]).first;
				this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, str);
			}

		}
	}
public:

	SearchList(tgui::Gui* gui, const int& curSet, int x, int y, int w, int h, bool *isWordMode) : x(x), y(y), w(w), h(h), curSet(curSet) {
		this->gui = gui;
		this->isWordModePtr = isWordMode;
		this->wordDetail = nullptr;	
		this->dataExec = &DataExecution::getInstance();
		this->favoriteList = new FavoriteList(this->gui, &wordDetail);

		this->initSearchButton();

		this->gui->get<tgui::Button>("btnHistory")->onClick([&, this] {
			if (this->curSet != this->dataExec->getCurDataset()) {
				this->curSet = this->dataExec->getCurDataset();
			}
			showHistory();
			});

		//showHistory();
		this->gui->get<tgui::EditBox>("SearchBar")->onTextChange([&, this]() {
			if(*this->isWordModePtr == true) onChangingText();
		});		
	

		this->gui->get<tgui::EditBox>("SearchBar")->onFocus([&, this]() {
			if(*this->isWordModePtr == true) onChangingText();
			});
		//this->wordDetail->setVisible(false);
	};

	void initSearchButton() {
		this->gui->get<tgui::Button>("btnSearch")->onClick([&, this]() {
			tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
			if (!checkValidString(text)) {
				this->gui->get<tgui::EditBox>("SearchBar")->setText("");
				return;
			}
			text = text.toLower();
			if (*this->isWordModePtr == true) {
				if (this->wordDetail) this->wordDetail->changeWord((string)text);
				else this->wordDetail = new WordDetail(this->gui, 25, 100, 450, 600, (string)text);
			} else {
				turnNonUnicodeString(text);
				text = text.toLower();
				auto nwDataId = this->dataExec->getKeysSubsequence((string)text);
				showSuggestions({}, nwDataId);
			}
			});
	}

	void onChangingText() {
		tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
		if (!checkValidString(text)) {
			clear();
			return;
		}
		tgui::String stdText;

		for (int i = 0; i < (int)text.length(); i++) {
			stdText.push_back(atomic_char32_t(text[i]));
		}
		bool check = turnNonUnicodeString(stdText);
		stdText = stdText.toLower();
		//cerr << stdText << '\n';
		if (stdText.length() < 1) {
			//cerr << "Type down more shit you idiot\n";
			this->showHistory();
		}
		else {
			auto nwData = this->dataExec->getListOfKeys((string)stdText, 8);
			//auto nwIds = this->dataExec->getKeySubarray(text.toStdString());
			showSuggestions(nwData);
		}
	}

	void clear() {
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

	void changeSearchSet(const int& curSet) {
		this->curSet = curSet;
	}

	bool checkSuggestion(string& str) {
		string getDef = this->dataExec->getDefinition(str);
		return getDef != "";
		
	}

	void showSuggestions(std::vector<std::string> nwData = {}, std::vector<int> nwDataIdx = {}) {
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

			if(*this->isWordModePtr == true) eb->setText(suggestedKeys[i]);
			else {
				auto tmp = this->dataExec->getData(nwDataIdx[i]);
				eb->setText(this->reduceStr(tmp.first + ": " + tmp.second, 80));
			}
			eb->setTextSize(16);
			eb->setTextPosition({ "2%", "40%" },{0, 0});
			
			//eb->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(50));

			eb->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordButton"));
			this->gui->add(eb);
			eb->onClick([i, this]() {
				setupWordDetail(i);
				});
			cnt++;
		}

		
		this->updateDecoration(cnt);
		
	};

	~SearchList() {
		if (this->wordDetail) {
			delete this->wordDetail;
		}
		if (this->favoriteList) {
			delete this->favoriteList;
		}

	}


	void update() {
		//std::cout << x->getWidgetName() << "\n";
		if (this->gui->getFocusedChild() == nullptr)
			showSuggestions({});
		this->favoriteList->update();
		if (this->wordDetail != NULL) {
			this->wordDetail->update();
		}
	}
};

