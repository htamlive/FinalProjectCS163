#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "GameState.h"
#include "SearchList.h"
#include "Trie.h"
#include "DATASET.h"
#include "State.h"
#include "BackgroundAnimations.h"
#include "FavoriteList.h"
class MainMenuState : public State
{
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;

	int curSet = 1;

	vector<DATASET*> dataSet;
	vector<Trie*> tries;
	vector<vector<string>> tmpDataSet;
	
	vector<string> btnNames = {"btnENtoEN", "btnENtoVIE","btnVIEtoEN" ,"btnSLANG", "btnEmoji" };
	SearchList* searchList;
	BackgroundAnimations* backgroundAnimations;
	FavoriteList* favoriteList;
	vector<string> data;
public:
	bool isWordMode = true;
	MainMenuState(sf::RenderWindow* window, std::vector<State*>* states) : State(window, states) {
		this->gui = new Gui(ref(*window));
		this->gui->loadWidgetsFromFile("Template/MenuTem.txt");
		initBackground();
		initTries({ "Dataset/FilterOxford.csv", "Dataset/FilterENtoVIEAgain.csv", "Dataset/slang.txt", "Dataset/emotional.txt" });
		initSearchBar();
		initButtons();
		initSearchButton();
		//cout << this->gui->get<tgui::Picture>("triag1")->getPosition().x << "\n";
		this->backgroundAnimations = new BackgroundAnimations(this->gui);
		this->favoriteList = new FavoriteList(this->gui);

		this->gui->get<tgui::ChildWindow>("ChildWindow")->onClosing([&](bool* abort) {
			*abort = true;
			this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(false);
			});
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImageScaling(0.8f);
		//this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		/*std::cout << this->gui->get<tgui::ListView>("ListView1")->addColumn("Hello");*/
		//this->gui->get<tgui::ListView>("ListView1")->setColumnText(0, "Hello");

		//this->gui->get<tgui::ListView>("ListView1")->setInheritedFont(tgui::Font("Template/fonts/UTM Androgyne.ttf"));
		//this->gui->get<tgui::ListView>("ListView1")->clic;
	}
	void initTries(vector<string> dataName) {
		for (int i = 0; i < (int)dataName.size(); i++) {
			DATASET* data = new DATASET(dataName[i]);
			data->loadData();
			dataSet.push_back(data);
			long long tmpDataIdx = this->tmpDataSet.size();
			loadTmpData();
			Trie* trie = new Trie();

			for (int j = 0; j < (int)data->Data.size(); j++) {
				pair<string, string> cur = data->Data[j];
				transform(cur.first.begin(), cur.first.end(), cur.first.begin(), ::tolower);
				for (int k = 0; k < (int)tmpDataSet[tmpDataIdx + j].size(); k++) {
					trie->addWord(cur.first, make_pair(tmpDataIdx + j, k));
				}
			}

			tries.push_back(trie);
		}
	}

	void initBackground() {
		this->background.setSize(sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		) 
		);
		this->backgroundTexture.loadFromFile("images/bg.png");
		this->background.setTexture(&this->backgroundTexture);
	}

	void initButtons() {
		this->gui->get<tgui::Button>("btnWordQuiz")->onClick([&, this]() {
			this->states->push_back(new GameState(this->window, this->states, 0, curSet));
			});		
		
		this->gui->get<tgui::Button>("btnDefQuiz")->onClick([&, this]() {
			this->states->push_back(new GameState(this->window, this->states, 1, curSet));
			});

		this->gui->get<tgui::Button>("btnCross")->onClick([&]() {
			this->resetSearchBar();
			});

		this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordDefWord"));

		this->gui->get<tgui::Button>("btnWordDef")->onClick([&]() {
			this->isWordMode ^= 1;
			if(this->isWordMode)
				this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{"Template/themes/MyThemes.txt"}.getRenderer("WordDefWord"));
			else
				this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{"Template/themes/MyThemes.txt"}.getRenderer("WordDefDef"));
			this->gui->get<tgui::Button>("btnWordDef")->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			this->gui->get<tgui::EditBox>("SearchBar")->setFocused(true);
			});
	}

	void initSearchBar() {
		this->searchList = new SearchList(this->gui, this->curSet, this->tries, &this->tmpDataSet, 550, 280, 720, 60);
		data = { "Hello", "Nice", "Helpful", "Helicopter"};
		this->searchList->update(data);
		this->isWordMode = true;

		this->gui->get<tgui::EditBox>("SearchBar")->onTextChange([&]() {
			updateSearchBar();
			});
	};

	vector<string> getListOfWords(string prefix, int maximum) {
		//cerr << this->tries.size() << '\n';
		return this->tries[this->curSet]->getListOfWords(prefix, maximum);
	}

	void initSearchButton() {
		this->gui->get<tgui::Button>("btnSearch")->onClick([&]() {
			//cerr << "In here : " << this->curSet << '\n';
			tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
			if (text.length() < 2) {
				cerr << "Type down more shit you idiot\n";
			}
			else {
				this->data = this->getListOfWords(text.toStdString(), 5);
				//cerr << this->data.size() << '\n';
				this->searchList->update(data);
			}
			});
	}


	void resetSearchBar() {
		this->gui->get<tgui::EditBox>("SearchBar")->setText("");

		this->gui->get<tgui::EditBox>("SearchBar")->setFocused(true);
	}

	~MainMenuState() {
		delete this->gui;
		delete this->backgroundAnimations;
		//delete this->tmpDataSet;
	};

	void updateInput(const float& dt) {

	}

	void updateEvents() {
		this->gui->handleEvent(this->ev);
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->endState();
			break;
		case sf::Event::TextEntered:

			//update
			break;
		default:
			break;
		}
	};

	void loadTmpData() {
		DATASET* data = this->dataSet.back();
		for (int i = 0; i < data->Data.size(); i++) {
			vector<string> arr;
			stringstream s(data->Data[i].second);
			string word;
			while (s >> word) {
				arr.push_back(word);
			}
			this->tmpDataSet.push_back(arr);
		}
	}

	void update(const float& dt) {
		this->updateBtns();
		this->favoriteList->update();
		this->backgroundAnimations->update(dt);
	};

	void updateSearchBar() {
		tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
		vector<string> nwData;
		for (auto x : this->data) {
			if (x.substr(0, text.size()) == text && text != "") {
				nwData.push_back(x);
			}
		}

		this->searchList->update(nwData);
		
	};

	void updateBtns() {
		for (int i = 0; i < this->btnNames.size(); ++i) {
			if (this->gui->get<tgui::Button>(btnNames[i])->isFocused() && i != this->curSet) {
				this->gui->get<tgui::Button>(btnNames[this->curSet])->leftMouseButtonNoLongerDown();
				this->curSet = i;
				this->searchList->changeSearchSet(this->curSet);
				this->gui->get<tgui::Button>(btnNames[this->curSet])->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			}
		}
		this->gui->get<tgui::Button>(this->btnNames[this->curSet])->leftMousePressed({});
	}

	void render(sf::RenderTarget* target = nullptr) {
		if (!target) {
			target = this->window;
		}
		target->draw(this->background);
		this->gui->draw();
	};
};

