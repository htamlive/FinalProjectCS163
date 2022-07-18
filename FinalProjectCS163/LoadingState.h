#pragma once
#include "State.h"
#include "DataExecution.h"
#include "BackgroundAnimations.h"

class LoadingState : public State {
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	BackgroundAnimations* bgAnimation;
	Entity* A, *B, *C;
	float cur = 0;
	bool showLogo = true;
	bool showName = true;
	void initBackground();
public:

	LoadingState(sf::RenderWindow* window, std::vector<State*>* states);

	void updateEvents();

	~LoadingState();

	void update(const float& dt) override;

	void updateInput(const float& dt) override;

	void render(sf::RenderTarget* target = nullptr) override;
};

