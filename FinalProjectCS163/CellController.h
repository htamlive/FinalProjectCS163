#pragma once
#include "Cell.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <string>

class CellController
{
private:
	float height, width, x, y, startTime, totalTime;
	int lastPlayTime;
	bool isOver, isWin, isFinish;
	Clock clock;

	std::vector<std::vector<Cell*>> cells;
	std::vector<std::vector<int>> test;
	std::vector<std::vector<float>> timeOpen;
	std::vector<std::vector<float>> timeFlag;


	tgui::Gui* gui;
	//CellAnimations* cellAnimations;
	//GameExecution* gameExec;
	//GameSounds* gameSound;
	//GameOptions* gameOptions;

	void initVariables() {
		clock.restart();
		this->isOver = false;
		this->isWin = false;
		this->isFinish = false;

		this->totalTime = 0;
		this->lastPlayTime = 0;
		this->startTime = -1;
	}

	void initOptions() {
		for (int i = 1; i <= 4; ++i) {
			;
			gui->get<tgui::Button>("btn" + std::to_string(i))->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("Quest" + std::to_string(i)));
			gui->get<tgui::Button>("btn" + std::to_string(i))->showWithEffect(tgui::ShowEffectType::Scale, sf::milliseconds(500));
		}
	}

	void showCorrect(int correctOpt = 1) {
		gui->get<tgui::Button>("btn" + std::to_string(correctOpt))->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("QuestAC"));
		gui->get<tgui::Button>("btn" + std::to_string(correctOpt))->showWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(100));
	}

	void showIncorrect(int inpOpt = 1) {
		gui->get<tgui::Button>("btn" + std::to_string(inpOpt))->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("QuestWA"));
	}

	void setEnabledOptions(bool flag) {
		for (int i = 1; i <= 4; ++i) {
			gui->get<tgui::Button>("btn" + std::to_string(i))->setEnabled(flag);
		}
	}

	void setUpGamePlay(int correctOpt = 1) {
		for (int i = 1; i <= 4; ++i) {
			gui->get<tgui::Button>("btn" + std::to_string(i))->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("Quest" + std::to_string(i)));
			gui->get<tgui::Button>("btn" + std::to_string(i))->showWithEffect(tgui::ShowEffectType::Scale, sf::milliseconds(500));
			if (i != correctOpt) {
				gui->get<tgui::Button>("btn" + std::to_string(i))->onClick([&,i,correctOpt, this]() {
					if (isFinish) return;
					isWin = false;
					isOver = true;
					isFinish = true;
					showCorrect(correctOpt);
					showIncorrect(i);
					setEnabledOptions(false);
					gui->get<tgui::Button>("btnNext")->setVisible(true);
					});
			}
		}
		gui->get<tgui::Button>("btn" + std::to_string(correctOpt))->onClick([&,correctOpt, this]() {
			if (isFinish) return;
			isWin = true;
			isFinish = true;
			showCorrect(correctOpt);
			setEnabledOptions(false);
			gui->get<tgui::Button>("btnNext")->setVisible(true);
			});

		
	}

	void getNextGame() {
		this->setEnabledOptions(true);
		setUpGamePlay(1);
		this->isFinish = false;
		this->gui->get<tgui::Button>("btnNext")->setVisible(false);
	}

public:
	
	virtual ~CellController() {

	};


	CellController(tgui::Gui*& gui, RenderWindow* window) {

		this->gui = gui;
		this->initVariables();
		this->initOptions();

		this->gui->get<tgui::EditBox>("ebPrompt")->setText("Hello hello hello");
		
		this->gui->get<tgui::Button>("btnNext")->onClick([&, this]() {
			getNextGame();
			});

		getNextGame();
	};

	CellController(tgui::Gui* gui, RenderWindow* window, int x, int y, float width, float height) {
		
		this->height = height;
		this->width = width;
		this->x = x;
		this->y = y;
		this->gui = gui;
		
		
		this->initVariables();
		this->initOptions();

	};


	bool checkStarted() {
		return this->startTime > 0;
	};

	bool checkWin() {
		return this->isOver && this->isWin;
	};

	bool checkLose() {
		return this->isOver && !this->isWin;
	};

	bool openCell(short row, short col);
	void setOpenCellStatus(int row, int col);
	void updateOpenCells();
	bool flagOrUnflagCell(short row, short col);

	void udpateMouseClick();
	void update() {

	};
	void render(RenderTarget* target);

	float getStartTime();
	float getPlayTime();

	float get_height();
	float get_width();

	int getCurrentFlags();
	int updateHighScore();
};

