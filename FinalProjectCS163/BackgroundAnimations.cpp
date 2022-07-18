#include "BackgroundAnimations.h"

void BackgroundAnimations::initVariables() {
	this->triag[0]->setTrajX(new Harmonic(400, PI / 3, PI));

	this->triag[1]->setTrajY(new Harmonic(200, PI / 3, PI));

	this->cross[0]->setRotate(new Harmonic(3, PI / 3, PI));

	this->cross[1]->setRotate(new Harmonic(3, PI / 3, PI));

	this->cross[2]->setRotate(new Harmonic(3, PI / 3, PI));
	this->cross[2]->setTrajY(new Harmonic(300, PI / 3, 0));

	this->cross[3]->setRotate(new Harmonic(3, PI / 3, PI));
	this->cross[3]->setTrajY(new Harmonic(300, PI / 3, PI));
}

BackgroundAnimations::BackgroundAnimations(tgui::Gui* gui) {
	this->gui = gui;
	for (int i = 0; i < 2; ++i) {
		triag[i] = new Entity(this->gui, "triag" + std::to_string(i + 1));
	}

	for (int i = 0; i < 4; ++i) {
		cross[i] = new Entity(this->gui, "cross" + std::to_string(i + 1));
	}

	initVariables();
}

BackgroundAnimations::~BackgroundAnimations() {
	for (int i = 0; i < 2; ++i) {
		delete triag[i];
	}

	for (int i = 0; i < 4; ++i) {
		delete cross[i];
	}
}

void BackgroundAnimations::update(const float& dt) {
	for (int i = 0; i < 2; ++i) {
		triag[i]->update(dt);
	}

	for (int i = 0; i < 4; ++i) {
		cross[i]->update(dt);
	}
}