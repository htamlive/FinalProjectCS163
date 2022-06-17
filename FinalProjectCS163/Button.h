#pragma once
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"

using namespace sf;

enum button_states { BTN_IDLE = 0, BTN_HOVER = 1, BTN_ACTIVE_L = 2, BTN_ACTIVE_R = 3 };
class MyButton
{
private:
	void updateByMouse(const sf::Vector2f mousePos);
	void updateType();

protected:
	short unsigned buttonState;

	RectangleShape shape;
	sf::Texture* texture;
	sf::Font* font;
	sf::Text text;
	Clock clock;
	float delay;


	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;


public:
	MyButton(float x, float y, float width, float height, sf::Font* font, std::string text, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
	~MyButton();

	bool isPressedLeft();
	bool isPressedRight();
	void setTextSize(int size);
	void setCenter();
	bool setTexture(std::string filePath);
	void setText(std::string text);
	void setPosition(sf::Vector2f pos);

	std::string getText();

	void update(const sf::Vector2f mousePos);
	void render(RenderTarget* target);
};


