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
#include "DataExecution.h"
class MainMenuState : public State
{
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;

	int curOpt = 1;

	DataExecution* dataExec;
	vector<DATASET*> dataSet;
	vector<Trie*> tries;
	vector<vector<string>> tmpDataSet;
	
	vector<string> btnNames = {"btnENtoEN", "btnENtoVIE","btnVIEtoEN" ,"btnSLANG", "btnEmoji" };
	vector<int> datasetId = { DATASETID::ENtoEN, DATASETID::ENtoVIE,DATASETID::VIEtoEN ,DATASETID::SLANG, DATASETID::EMOJI };
	
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

		initSearchBar();
		initButtons();
		initSearchButton();

		this->backgroundAnimations = new BackgroundAnimations(this->gui);
		this->favoriteList = new FavoriteList(this->gui);

		this->gui->get<tgui::ChildWindow>("ChildWindow")->onClosing([&](bool* abort) {
			*abort = true;
			this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(false);
			});
		this->gui->get<tgui::ChildWindow>("ChildWindow")->get<tgui::BitmapButton>("Button1")->setImageScaling(0.8f);

		this->dataExec = &DataExecution::getInstance();

		this->dataExec->loadKeys(DATASETID::ENtoVIE, true);

		//this->gui->get<tgui::ChildWindow>("ChildWindow")->setVisible(true);
		/*std::cout << this->gui->get<tgui::ListView>("ListView1")->addColumn("Hello");*/
		//this->gui->get<tgui::ListView>("ListView1")->setColumnText(0, "Hello");

		//this->gui->get<tgui::ListView>("ListView1")->setInheritedFont(tgui::Font("Template/fonts/UTM Androgyne.ttf"));
		//this->gui->get<tgui::ListView>("ListView1")->clic;
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
			this->states->push_back(new GameState(this->window, this->states, 0, datasetId[curOpt]));
			});		
		
		this->gui->get<tgui::Button>("btnDefQuiz")->onClick([&, this]() {
			this->states->push_back(new GameState(this->window, this->states, 1, datasetId[curOpt]));
			});


		this->gui->get<tgui::Button>("btnCross")->onClick([&]() {
			this->resetSearchBar();
			});

		this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordDefWord"));

		this->gui->get<tgui::Button>("btnWordDef")->onClick([&, this]() {
			this->isWordMode ^= 1;
			if (this->isWordMode) {
				this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordDefWord"));
				dataExec->loadKeys(datasetId[curOpt]);
			}	
			else {
				this->gui->get<tgui::Button>("btnWordDef")->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("WordDefDef"));
				dataExec->loadDefs(datasetId[curOpt]);
			}	

			this->gui->get<tgui::Button>("btnWordDef")->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			this->gui->get<tgui::EditBox>("SearchBar")->setFocused(true);
		
			});	

		this->gui->get<tgui::Button>("btnRestore")->onClick([&, this]() {
			dataExec->restore(datasetId[curOpt]);
			searchList->onChangingText();
			});
	}

	void initSearchBar() {
		this->searchList = new SearchList(this->gui, this->datasetId[this->curOpt], this->tries, &this->tmpDataSet, 550, 280, 720, 60);
		this->isWordMode = true;
		//data = { "Hello", "Nice", "Helpful", "Helicopter"};
		//this->searchList->update(data);
		

		//this->gui->get<tgui::EditBox>("SearchBar")->onTextChange([&]() {
		//	updateSearchBar();
		//	});
	};

	vector<string> getListOfWords(string prefix, int maximum) {
		return this->dataExec->getListOfWords(prefix, maximum);
	}

	void initSearchButton() {
		this->gui->get<tgui::Button>("btnSearch")->onClick([&]() {
			//cerr << "In here : " << this->curSet << '\n';
			tgui::String text = this->gui->get<tgui::EditBox>("SearchBar")->getText();
			text = text.toLower();
			if (text.length() < 1) {
				cerr << "Type down more shit you idiot\n";
			}
			else {
				this->data = this->dataExec->getListOfWords(text.toStdString(), 8);
				this->searchList->showSuggestions(data);
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
		this->searchList->update();
	};

	void updateBtns() {
		for (int i = 0; i < this->btnNames.size(); ++i) {
			if (this->gui->get<tgui::Button>(btnNames[i])->isFocused() && i != this->curOpt) {
				this->gui->get<tgui::Button>(btnNames[this->curOpt])->leftMouseButtonNoLongerDown();
				this->curOpt = i;
				this->searchList->changeSearchSet(this->datasetId[this->curOpt]);

				
				this->dataExec->loadDataset(this->datasetId[i], true);
				this->gui->get<tgui::Button>(btnNames[this->curOpt])->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(300));
			}
		}
		this->gui->get<tgui::Button>(this->btnNames[this->curOpt])->leftMousePressed({});
	}

	void render(sf::RenderTarget* target = nullptr) {
		if (!target) {
			target = this->window;
		}
		target->draw(this->background);
		this->gui->draw();
	};
};

