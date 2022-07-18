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
#include "LoadingState.h"
class MainMenuState : public State {
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;

	int curOpt = 1;

	DataExecution* dataExec;
	
	vector<string> btnNames = {"btnENtoEN", "btnENtoVIE","btnVIEtoEN" ,"btnSLANG", "btnEmoji" };
	vector<int> datasetId = { DATASETID::ENtoEN, DATASETID::ENtoVIE,DATASETID::VIEtoEN ,DATASETID::SLANG, DATASETID::EMOJI };
	
	SearchList* searchList;
	BackgroundAnimations* backgroundAnimations;

	vector<string> data;
public:
	bool isWordMode = true;
	MainMenuState(sf::RenderWindow* window, std::vector<State*>* states);

	void initBackground();

	void initButtons();

	void initSearchBar();


	void resetSearchBar();

	~MainMenuState();

	void updateInput(const float& dt);

	void updateEvents();

	void update(const float& dt);

	void updateBtns();

	void render(sf::RenderTarget* target = nullptr);
};

