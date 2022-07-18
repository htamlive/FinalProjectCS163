#pragma once
#include "CellController.h"
#include "State.h"
#include "BackgroundAnimations.h"

enum GAMETYPE {
	GUESSWORD = 0,
	GUESSDEF = 1,
};


class GameState : public State {
private:
	vector<string> gametypes = { "Guess Words Quiz:", "Guess Definitions Quiz:" };
	vector<string> datasets = {"EMOJI", "SLANG", "EN TO VIE", "EN TO EN", "VIE TO EN" };

	sf::Texture backgroundTexture;
	RectangleShape background;

	tgui::Font utmAndro;

	int curLv, rows, columns, mines, currentFlags;
	int curGameType, curDataset;
	float height, width;
	bool lastPlay;

	CellController* cellController;

	BackgroundAnimations* backgroundAnimations;

	void initButtons();

	void initBackground();

	void initTitle();

	std::string getMode();

public:
	GameState(RenderWindow* window, std::vector<State*>* states, int gameType = GAMETYPE::GUESSWORD, int dataset = 0);
	
	virtual ~GameState();

	void updateMousePos() override;

	void updateEvents() override;

	void updateInput(const float& dt) override;

	void update(const float& dt) override;

	void render(sf::RenderTarget* target = nullptr) override;
};

