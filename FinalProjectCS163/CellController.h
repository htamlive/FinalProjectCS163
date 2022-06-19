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
	int lastPlayTime, tot, correct;
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

		this->totalTime = 0;
		this->lastPlayTime = 0;
		this->startTime = -1;

		this->tot = this->correct = 0;
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

	void addCorrect() {
		this->correct++;
	}

	void updateScore() {
		this->tot += 1;
		this->gui->get<tgui::Button>("btnTotal")->setText(std::to_string(this->tot));
		this->gui->get<tgui::EditBox>("ebCorrect")->setText(std::to_string(this->correct));
		this->gui->get<tgui::EditBox>("ebIncorrect")->setText(std::to_string(this->tot - this->correct));
		
	}

	void setUpGamePlay(int correctOpt = 1) {
		for (int i = 1; i <= 4; ++i) {
			gui->get<tgui::Button>("btn" + std::to_string(i))->onPress.disconnectAll();
			gui->get<tgui::Button>("btn" + std::to_string(i))->setRenderer(tgui::Theme{ "Template/themes/MyThemes.txt" }.getRenderer("Quest" + std::to_string(i)));
			gui->get<tgui::Button>("btn" + std::to_string(i))->showWithEffect(tgui::ShowEffectType::Scale, sf::milliseconds(500));
			
			if (i != correctOpt) {
				gui->get<tgui::Button>("btn" + std::to_string(i))->onPress([&,i,correctOpt, this]() {
					showCorrect(correctOpt);
					showIncorrect(i);
					setEnabledOptions(false);
					//std::cout << 1 << "\n";
					gui->get<tgui::Button>("btnNext")->setVisible(true);
					updateScore();
					});
			}
		}
		gui->get<tgui::Button>("btn" + std::to_string(correctOpt))->onPress([&,correctOpt, this]() {
			showCorrect(correctOpt);
			setEnabledOptions(false);
			gui->get<tgui::Button>("btnNext")->setVisible(true);
			addCorrect();
			updateScore();
			});

		
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

	void getNextGame() {
		this->setEnabledOptions(true);
		setUpGamePlay(1);
		this->gui->get<tgui::Button>("btnNext")->setVisible(false);
	}

	bool checkStarted() {
		return this->startTime > 0;
	};

};

