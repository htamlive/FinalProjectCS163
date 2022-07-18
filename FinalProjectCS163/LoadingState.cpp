#include "LoadingState.h"

void LoadingState::initBackground() {
	this->background.setSize(sf::Vector2f(
		static_cast<float>(this->window->getSize().x),
		static_cast<float>(this->window->getSize().y)
	)
	);
	this->backgroundTexture.loadFromFile("images/bg.png");
	this->background.setTexture(&this->backgroundTexture);
}

LoadingState::LoadingState(sf::RenderWindow* window, std::vector<State*>* states) : State(window, states) {
	this->gui = new Gui(ref(*window));
	this->gui->loadWidgetsFromFile("Template/LoadingTem.txt");
	initBackground();

	this->gui->get<tgui::ProgressBar>("LoadingBar")->setMaximum(100);
	this->gui->get<tgui::ProgressBar>("LoadingBar")->setMinimum(0);
	this->gui->get<tgui::Picture>("logoBrand")->showWithEffect(tgui::ShowEffectType::Fade, 200);


	this->bgAnimation = new BackgroundAnimations(this->gui);

	A = new Entity(this->gui, "picA", nullptr, new Harmonic(20, PI / 3, 0), nullptr);
	B = new Entity(this->gui, "picB", nullptr, new Harmonic(20, PI / 3, PI / 3), nullptr);
	C = new Entity(this->gui, "picC", nullptr, new Harmonic(20, PI / 3, 2 * PI / 3), nullptr);
}

void LoadingState::updateEvents() {
	this->gui->handleEvent(this->ev);
	switch (this->ev.type)
	{
	case sf::Event::Closed:

		break;
	case sf::Event::TextEntered:

		//update
		break;
	default:
		break;
	}
};

LoadingState::~LoadingState() {
	delete this->gui;
	delete this->bgAnimation;
	delete A;
	delete B;
	delete C;
}

void LoadingState::update(const float& dt) {
	if (DataExecution::getInstance().checkFinishAll()) {
		if (cur > 110) this->endState();
		if (cur > 70 && cur < 85) cur += PI;
	}
	cur += 10.5 * dt;
	if (cur > 30 && this->showLogo) {
		this->showLogo = false;
		this->gui->get<tgui::Picture>("logoBrand")->hideWithEffect(tgui::ShowEffectType::Fade, 100);
		cur += PI;
	}
	else if (cur > 40 && !this->showLogo && this->showName) {
		this->showName = false;
		this->gui->get<tgui::Picture>("appname")->showWithEffect(tgui::ShowEffectType::Fade, 100);
		this->gui->get<tgui::Picture>("picA")->showWithEffect(tgui::ShowEffectType::Fade, 100);
		this->gui->get<tgui::Picture>("picB")->showWithEffect(tgui::ShowEffectType::Fade, 100);
		this->gui->get<tgui::Picture>("picC")->showWithEffect(tgui::ShowEffectType::Fade, 100);

	}
	this->bgAnimation->update(dt);
	this->A->update(dt);
	this->B->update(dt);
	this->C->update(dt);
	this->gui->get<tgui::ProgressBar>("LoadingBar")->setValue(cur);
	this->gui->get<tgui::Label>("lblLoadingTitle")->setInheritedOpacity(cos(this->cur / 10) * cos(this->cur / 10));
	this->gui->get<tgui::Label>("lblLoading")->setText(std::to_string(std::min(cur, (float)100.0)) + " %");
}

void LoadingState::updateInput(const float& dt) {

}

void LoadingState::render(sf::RenderTarget* target) {
	if (!target) {
		target = this->window;
	}
	target->draw(this->background);

	this->gui->draw();
};