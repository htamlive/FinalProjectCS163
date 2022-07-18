#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <thread>

#include "MainMenuState.h"
#include "LoadingState.h"
#include "State.h"

using namespace sf;

class Dictionary
{
private:
	RenderWindow* window;
	VideoMode videoMode;
	sf::Event ev;
	std::vector<VideoMode> videoModes;
	ContextSettings windowSettings;
	bool fullscreen;

	Clock dtClock;
	float dt;

	std::vector<State*> states;


	void initVariables();
	void initWindow();
	void initState();

	void updateEvents();
	void updateDt();

public:
	Dictionary();
	virtual ~Dictionary();

	const bool running() const;

	void endApplication();

	void updateSFMLEvents();
	void update();

	void render();
	void run();

	void closeWindow();
	void keyReleasedProc();
	void mouseProc();
};

