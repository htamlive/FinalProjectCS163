#pragma once
#include "Entity.h"
#include <iostream>
#include <string>

class BackgroundAnimations {
private:
	Entity* triag[2], * cross[4];
	tgui::Gui* gui;
	
	void initVariables();

public:
	BackgroundAnimations(tgui::Gui* gui);

	virtual ~BackgroundAnimations();

	void update(const float& dt);

};

