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

struct Harmonic
{
	float r, w, cur, offset;
	Harmonic(float r, float w, float offset = PI / 2) : r(r), w(w), cur(0), offset(offset) {};
	float newPos(const float& dt) {
		cur += dt*w;
		if (cur > 2 * PI) cur -= 2*PI;
		return r * cos(cur + offset);
	}
};

class Entity
{
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
	Entity(tgui::Gui* gui, std::string name) {
		this->gui = gui;
		this->name = name;
		this->initPos = this->gui->get<tgui::Picture>(name)->getPosition();

		this->gui->get<tgui::Picture>(name)->setPosition(this->initPos);
		trajX = trajY = rot = nullptr;

		//trajX = new Harmonic(200, PI/3, PI);
		//rot = new Harmonic(100, PI/3, PI/4);
		//this->pic->moveWithAnimation({ 2.0,3.5 },sf::microseconds(1000));
	};

	Entity(tgui::Gui* gui, std::string name, Harmonic* trajX, Harmonic* trajY, Harmonic* rot) {
		this->gui = gui;
		this->name = name;
		this->initPos = this->gui->get<tgui::Picture>(name)->getPosition();

		this->gui->get<tgui::Picture>(name)->setPosition(this->initPos);
		this->trajX = trajX;
		this->trajY = trajY;
		this->rot = rot;

		//trajX = new Harmonic(200, PI/3, PI);
		//rot = new Harmonic(100, PI/3, PI/4);
		//this->pic->moveWithAnimation({ 2.0,3.5 },sf::microseconds(1000));
	};

	virtual ~Entity() {
		if (trajX)  delete trajX;
		if (rot) delete rot;
		if (trajY) delete trajY;
	};
	
	virtual void update(const float& dt) {
		if(trajX) this->gui->get<tgui::Picture>(name)->moveWithAnimation({ this->initPos.x + trajX->newPos(dt), this->initPos.y}, sf::seconds(dt));
		if(trajY) this->gui->get<tgui::Picture>(name)->moveWithAnimation({ this->initPos.x, this->initPos.y + trajY->newPos(dt) }, sf::seconds(dt));
		if(rot) this->gui->get<tgui::Picture>(name)->setRotation(this->curRot += this->rot->newPos(dt), {0.5, 0.5});
	};

	void setTrajX(Harmonic* trajX) {
		if (this->trajX) delete this->trajX;
		this->trajX = trajX;
	}

	void setTrajY(Harmonic* trajY) {
		if (this->trajY) delete this->trajY;
		this->trajY = trajY;
	}

	void setRotate(Harmonic* rot) {
		if (this->rot) delete this->rot;
		this->rot = rot;
	}
};

