#include "GameState.h"

void GameState::initButtons() {
	this->gui->get<tgui::Button>("btnBack")->onClick([&]() {
		this->endState();
		});
}

void GameState::initBackground() {
	this->background.setSize(sf::Vector2f(
		static_cast<float>(this->window->getSize().x),
		static_cast<float>(this->window->getSize().y)
	)
	);
	this->backgroundTexture.loadFromFile("images/bg.png");
	this->background.setTexture(&this->backgroundTexture);
};

void GameState::initTitle() {
	utmAndro = tgui::Font("Template/fonts/UTM Androgyne.ttf");

	this->gui->get<tgui::Label>("lbGameType")->setInheritedFont(utmAndro);
	this->gui->get<tgui::Label>("lbGameType")->setText(this->gametypes[this->curGameType]);

	this->gui->get<tgui::Label>("lbDataset")->setInheritedFont(utmAndro);
	this->gui->get<tgui::Label>("lbDataset")->setText(this->datasets[this->curDataset]);
}

std::string GameState::getMode() {
	return (this -> gametypes)[this -> curGameType];
};

GameState::GameState(RenderWindow* window, std::vector<State*>* states, int gameType, int dataset) : State(this->window, this->states) {
	this->window = window;
	this->curGameType = gameType;
	this->curDataset = dataset;
	DataExecution::getInstance().loadDataset(this->curDataset, true);
	this->gui = new Gui(*window);
	this->gui->loadWidgetsFromFile("Template/GameTem.txt");
	this->initBackground();
	this->initButtons();
	cellController = new CellController(this->gui, this->window, gameType);

	this->backgroundAnimations = new BackgroundAnimations(this->gui);
	this->initTitle();

};

GameState::~GameState() {
	delete this->gui;
	delete this->cellController;
};

void GameState::updateMousePos() {};

void GameState::updateEvents() {
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
void GameState::updateInput(const float& dt) {};

void GameState::update(const float& dt) {
	this->backgroundAnimations->update(dt);
}

void GameState::render(sf::RenderTarget* target) {
	if (!target) {
		target = this->window;
	}
	target->draw(this->background);
	this->gui->draw();
};