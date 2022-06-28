#pragma once
#include "State.h"
#include "DataExecution.h"
#include "BackgroundAnimations.h"

class LoadingState : public State
{
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	BackgroundAnimations* bgAnimation;
	float cur = 0;
	void initBackground() {
		this->background.setSize(sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
		);
		this->backgroundTexture.loadFromFile("images/bg.png");
		this->background.setTexture(&this->backgroundTexture);
	}
public:

	LoadingState(sf::RenderWindow* window, std::vector<State*>* states) : State(window, states) {
		this->gui = new Gui(ref(*window));
		this->gui->loadWidgetsFromFile("Template/LoadingTem.txt");
		initBackground();

		this->gui->get<tgui::ProgressBar>("LoadingBar")->setMaximum(100);
		this->gui->get<tgui::ProgressBar>("LoadingBar")->setMinimum(0);
		
		this->bgAnimation = new BackgroundAnimations(this->gui);
	}

	void updateEvents() {
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

	~LoadingState() {
		delete this->gui;
		delete this->bgAnimation;
	}

	void update(const float& dt) override {
		if (DataExecution::getInstance().checkFinishAll() && cur > 120) {
			this->endState();
		}
		cur += 10.5 * dt;
		this->bgAnimation->update(dt);
		this->gui->get<tgui::ProgressBar>("LoadingBar")->setValue(cur);
		this->gui->get<tgui::Label>("lblLoadingTitle")->setInheritedOpacity(cos(this->cur/10)* cos(this->cur / 10));
		this->gui->get<tgui::Label>("lblLoading")->setText(std::to_string(std::min(cur, (float)100.0)) + " %");
	}

	void updateInput(const float& dt) override {

	}

	void render(sf::RenderTarget* target = nullptr) override {
		if (!target) {
			target = this->window;
		}
		target->draw(this->background);
		this->gui->draw();
	};
};

