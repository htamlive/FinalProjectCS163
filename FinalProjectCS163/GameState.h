#pragma once
#include "CellController.h"
#include "State.h"
class GameState : public State
{
private:
	sf::Texture backgroundTexture;
	RectangleShape background;

	sf::Font font;

	//StatusBar* statusBar;

	int curLv, rows, columns, mines, currentFlags;
	int curGameType;
	float height, width;
	bool lastPlay;

	CellController* cellController;

	void initVariables(int rows = 8, int cols = 10, int mines = 10);
	void initFonts();
	void initCells();
	void initStatusBar();
	void initKeyBinds();

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
		this->backgroundTexture.loadFromFile("images/bg1.png");
		this->background.setTexture(&this->backgroundTexture);
	};

	std::string getMode();

public:
	GameState(RenderWindow* window, std::vector<State*>* states, int gameType = 0) : State(this->window, this->states) {
		this->window = window;
		this->curGameType = gameType;
		this->gui = new Gui(ref(*window));
		this->gui->loadWidgetsFromFile("Template/GameTem.txt");
		this->initBackground();
		this->initButtons();
		cellController = new CellController(this->gui, this->window);

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
		this->cellController->update();
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

