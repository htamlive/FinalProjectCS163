#include "Entity.h"

Harmonic::Harmonic(float r, float w, float offset) : r(r), w(w), cur(0), offset(offset) {};

float Harmonic::newPos(const float& dt) {
	cur += dt * w;
	if (cur > 2 * PI) cur -= 2 * PI;
	return r * cos(cur + offset);
}

void Entity::initVariables() {}

Entity::Entity(tgui::Gui* gui, std::string name) {
	this->gui = gui;
	this->name = name;
	this->initPos = this->gui->get<tgui::Picture>(name)->getPosition();

	this->gui->get<tgui::Picture>(name)->setPosition(this->initPos);
	trajX = trajY = rot = nullptr;
};

Entity::Entity(tgui::Gui* gui, std::string name, Harmonic* trajX, Harmonic* trajY, Harmonic* rot) {
	this->gui = gui;
	this->name = name;
	this->initPos = this->gui->get<tgui::Picture>(name)->getPosition();

	this->gui->get<tgui::Picture>(name)->setPosition(this->initPos);
	this->trajX = trajX;
	this->trajY = trajY;
	this->rot = rot;
};

Entity::~Entity() {
	if (trajX)  delete trajX;
	if (rot) delete rot;
	if (trajY) delete trajY;
};

void Entity::update(const float& dt) {
	if (trajX) this->gui->get<tgui::Picture>(name)->moveWithAnimation({ this->initPos.x + trajX->newPos(dt), this->initPos.y }, sf::seconds(dt));
	if (trajY) this->gui->get<tgui::Picture>(name)->moveWithAnimation({ this->initPos.x, this->initPos.y + trajY->newPos(dt) }, sf::seconds(dt));
	if (rot) this->gui->get<tgui::Picture>(name)->setRotation(this->curRot += this->rot->newPos(dt), { 0.5, 0.5 });
};

void Entity::setTrajX(Harmonic* trajX) {
	if (this->trajX) delete this->trajX;
	this->trajX = trajX;
}

void Entity::setTrajY(Harmonic* trajY) {
	if (this->trajY) delete this->trajY;
	this->trajY = trajY;
}

void Entity::setRotate(Harmonic* rot) {
	if (this->rot) delete this->rot;
	this->rot = rot;
}