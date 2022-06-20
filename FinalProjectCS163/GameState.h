#pragma once
#include "CellController.h"
#include "State.h"
#include "BackgroundAnimations.h"

enum GAMETYPE
{
	GUESSWORD = 0,
	GUESSDEF = 1,
};


class GameState : public State
{
private:
	vector<string> gametypes = { "Guess Words Quiz:", "Guess Definitions Quiz:" };
	vector<string> datasets = { "EN TO EN", "EN TO VIE", "VIE TO EN", "SLANG", "EMOJI" };

	sf::Texture backgroundTexture;
	RectangleShape background;

	tgui::Font utmAndro;

	//StatusBar* statusBar;

	int curLv, rows, columns, mines, currentFlags;
	int curGameType, curDataset;
	float height, width;
	bool lastPlay;

	CellController* cellController;

	BackgroundAnimations* backgroundAnimations;

	//void initCells();
	//void initStatusBar();
	//void initKeyBinds();

	void initButtons() {
		this->gui->get<tgui::Button>("btnBack")->onClick([&]() {
			this->endState();
			});
	}

	void initBackground() {
		this->background.setSize(sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
		);
		this->backgroundTexture.loadFromFile("images/bg.png");
		this->background.setTexture(&this->backgroundTexture);
	};

	void initTitle() {
		utmAndro = tgui::Font("Template/fonts/UTM Androgyne.ttf");

		this->gui->get<tgui::Label>("lbGameType")->setInheritedFont(utmAndro);
		this->gui->get<tgui::Label>("lbGameType")->setText(this->gametypes[this->curGameType]);

		this->gui->get<tgui::Label>("lbDataset")->setInheritedFont(utmAndro);
		this->gui->get<tgui::Label>("lbDataset")->setText(this->datasets[this->curDataset]);
	}

	std::string getMode();

public:
	GameState(RenderWindow* window, std::vector<State*>* states, int gameType = GAMETYPE::GUESSWORD, int dataset = 0) : State(this->window, this->states) {
		this->window = window;
		this->curGameType = gameType;
		this->curDataset = dataset;
		this->gui = new Gui(ref(*window));
		this->gui->loadWidgetsFromFile("Template/GameTem.txt");
		this->initBackground();
		this->initButtons();
		cellController = new CellController(this->gui, this->window);

		this->backgroundAnimations = new BackgroundAnimations(this->gui);
		this->initTitle();
		
	};
	//GameState(RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	//GameState(RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states, GameOptions* gameOptions);
	//GameState(RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states, int rows, int cols, int mines, int curLv);
	//GameState(RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states, int rows, int cols, int mines, int curLv, GameOptions* gameOptions);
	virtual ~GameState() {
		delete this->gui;
		delete this->cellController;
	};

	//void adjustCells(const int totalX, const int totalY);
	//sf::Vector2u setCenter(const int totalX, const int totalY);

	//bool checkLastPlay();
	void updateMousePos() override {

	};
	void updateEvents() override {
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
	void updateInput(const float& dt) override {

	};

	void update(const float& dt) override {
		this->backgroundAnimations->update(dt);
	}
	//void updateCells();
	//void updateStatusBar();
	//void update(const float& dt);
	//void renderCells(RenderTarget* target);
	//void renderStatusBar(RenderTarget* target);
	void render(sf::RenderTarget* target = nullptr) override {
		if (!target) {
			target = this->window;
		}
		target->draw(this->background);
		this->gui->draw();
	};
};

