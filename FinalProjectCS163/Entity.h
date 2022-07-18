#pragma once
#include <vector>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

using namespace sf;

const float PI = acos(-1);

struct Harmonic {
	float r, w, cur, offset;
	Harmonic(float r, float w, float offset = PI / 2);
	float newPos(const float& dt);
};

class Entity {
private:
	void initVariables();
protected:
	tgui::Picture::Ptr pic;
	tgui::Gui* gui;
	float movementSpeed;
	tgui::Vector2f initPos;
	float curRot = 0;
	std::string name;

	Harmonic* trajX, *rot, *trajY;
public:
	Entity(tgui::Gui* gui, std::string name);

	Entity(tgui::Gui* gui, std::string name, Harmonic* trajX, Harmonic* trajY, Harmonic* rot);

	virtual ~Entity();
	
	virtual void update(const float& dt);

	void setTrajX(Harmonic* trajX);

	void setTrajY(Harmonic* trajY);

	void setRotate(Harmonic* rot);
};

