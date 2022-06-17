#pragma once
#include "State.h"
#include <string>
#include <vector>
#include "GameState.h"
#include "SearchList.h"
class MainMenuState : public State
{
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;
	int curOption = 0;
	vector<string> btnNames = {"btnENtoEN", "btnENtoVIE","btnVIEtoEN" ,"btnSLANG", "btnEmoji" };
	SearchList* searchList;
public:
	MainMenuState(sf::RenderWindow* window, std::vector<State*>* states) : State(window, states) {
		this->gui = new Gui(ref(* window));
		this->gui->loadWidgetsFromFile("Template/MenuTem.txt");
		initBackground();
		initButtons();
		this->searchList = new SearchList(this->gui, 550, 280, 720, 60);
		this->searchList->update({ "Hello", "Nice", "To", "Meet", "You" });
	};

	void initBackground() {
		this->background.setSize(sf::Vector2f(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		) 
		);
		this->backgroundTexture.loadFromFile("images/bg1.png");
		this->background.setTexture(&this->backgroundTexture);
	}

	void initButtons() {
		this->gui->get<tgui::Button>("btnWordQuiz")->onClick([&]() {
			this->states->push_back(new GameState(this->window, this->states, 0));
			});		
		
		this->gui->get<tgui::Button>("btnDefQuiz")->onClick([&]() {
			this->states->push_back(new GameState(this->window, this->states, 1));
			});

		this->gui->get<tgui::Button>("btnCross")->onClick([&]() {
			this->resetSearchBar();
			});

	}


	void resetSearchBar() {
		this->gui->get<tgui::EditBox>("SearchBar")->setText("");
		
		
		this->gui->get<tgui::EditBox>("SearchBar")->setFocused(true);
	}

	~MainMenuState() {
		delete this->gui;
	};

	void updateInput(const float& dt) {

	}

	void updateEvents() {
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
	}

	void update(const float& dt) {

		for (int i = 0; i < this->btnNames.size(); ++i) {
			if (this->gui->get<tgui::Button>(btnNames[i])->isFocused() && i != this->curOption) {
				this->gui->get<tgui::Button>(btnNames[this->curOption])->leftMouseButtonNoLongerDown();
				this->curOption = i;
			}
		}
		this->gui->get<tgui::Button>(btnNames[this->curOption])->leftMousePressed({});
	
	}

	void render(sf::RenderTarget* target = nullptr) {
		if (!target) {
			target = this->window;
		}
		target->draw(this->background);
		this->gui->draw();
	}
};

