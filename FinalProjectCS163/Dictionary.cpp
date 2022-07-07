#include "Dictionary.h"
#include "MainMenuState.h"
#include "LoadingState.h"

void Dictionary::initVariables()
{
	this->window = nullptr;
	this->dt = 0.f;
	this->fullscreen = false;
}

void Dictionary::initWindow()
{

	this->videoModes = VideoMode::getFullscreenModes();

	this->videoMode.height = 800;
	this->videoMode.width = 1300;
	//{1300, 800}
	unsigned antialiasing_level = 0;

	this->fullscreen = false;
	this->windowSettings.antialiasingLevel = antialiasing_level;
	if (this->fullscreen) {
		this->window = new RenderWindow(this->videoMode, "Dictionary", Style::Fullscreen, windowSettings);
	}
	else {

		std::string name;
		name += "Dictionary";
		this->window = new RenderWindow(this->videoMode, "Dictionary", Style::Titlebar | Style::Close, windowSettings);

	}

	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);


}

void Dictionary::initState()
{
	this->states.push_back(new MainMenuState(this->window, &this->states));
	this->states.push_back(new LoadingState(this->window, &this->states));
}


Dictionary::Dictionary() {
	this->initVariables();
	this->initWindow();
	this->initState();
}

Dictionary::~Dictionary() {
	delete this->window;

	while (!this->states.empty()) {
		delete this->states.back();
		this->states.pop_back();
	}
}

const bool Dictionary::running() const
{
	return this->window->isOpen();
}

void Dictionary::updateEvents()
{
	switch (this->ev.type)
	{
	case sf::Event::Closed:
		closeWindow();
		break;
	case sf::Event::KeyReleased:
		keyReleasedProc();
		break;
	case sf::Event::MouseButtonReleased:
		mouseProc();
	}
}

void Dictionary::endApplication()
{
	//std::cout << "Ending application!\n";
}

void Dictionary::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();

	//system("cls");
	//std::cout << this->dt << "\n";
}


void Dictionary::updateSFMLEvents()
{
	if (!this->states.empty()) {
		while (this->window->pollEvent(this->states.back()->ev)) this->states.back()->updateEvents();
	}
	else {
		while (this->window->pollEvent(this->ev)) this->updateEvents();
	}
}

void Dictionary::update()
{

	this->updateSFMLEvents();
	if (!this->states.empty()) {
		this->states.back()->update(this->dt);
		if (this->states.back()->getQuit()) {
			this->states.back()->endState();
			delete this->states.back();
			this->states.pop_back();

		}
	}
	else {
		this->endApplication();
		this->window->close();
	}
}

void Dictionary::render()
{
	this->window->clear();
	if (!this->states.empty()) {
		this->states.back()->render();
	}
	this->window->display();
}

void Dictionary::run()
{
	while (this->running())
	{
		this->updateDt();
		this->update();
		this->render();
	}
	
	DataExecution::getInstance().setShutDown();
}

void Dictionary::closeWindow()
{
	this->window->close();
}

void Dictionary::keyReleasedProc()
{
	switch (ev.key.code)
	{
	case Keyboard::Enter:
		//field.restart();

	case Keyboard::Escape:
		this->window->close();
	}
}

void Dictionary::mouseProc()
{
	switch (this->ev.mouseButton.button)
	{
	case Mouse::Left:
		std::cout << "\nLeft click " << Mouse::getPosition(*this->window).x << " " << Mouse::getPosition(*this->window).y;
		//field.open_cell(mouse_cell_x, mouse_cell_y);

		break;
	case sf::Mouse::Right:
		std::cout << "\nRight click";
		//field.flag_cell(mouse_cell_x, mouse_cell_y);
	}
}

