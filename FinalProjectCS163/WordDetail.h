#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

using namespace tgui;
using namespace std;

class WordDetail
{
private:
	int x, y, w, h;
	tgui::Gui* gui;

public:
	bool on = false;
	WordDetail(tgui::Gui* GUI, int x, int y, int w, int h, string& str) : x(x), y(y), w(w), h(h) {
		this->gui = GUI;

		auto chWindow = tgui::ChildWindow::create();
		chWindow->setWidgetName("chWindow");
		chWindow->setPosition(x, y);
		chWindow->setSize(w, h);

		auto editBoxK = tgui::EditBox::create();
		editBoxK->setWidgetName("editBoxKeyWord");
		editBoxK->setPosition(150, 300);
		editBoxK->setSize(150, 30);
		editBoxK->setTextSize(14);
		editBoxK->setReadOnly();
		editBoxK->setAlignment(tgui::EditBox::Alignment::Center);
		editBoxK->setText(tgui::String(str));
		chWindow->add(editBoxK);

		auto editBoxD = tgui::TextArea::create();
		editBoxD->setWidgetName("editBoxDefinition");
		editBoxD->setPosition(75, 350);
		editBoxD->setSize(300, 200);
		editBoxD->setTextSize(14);
		//editBoxD->limitTextWidth();
		//editBoxD->setReadOnly();
		editBoxD->setDefaultText("Definition goes here");
		chWindow->add(editBoxD);

		this->gui->add(chWindow);

		on = true;
	}

	void removeWindow() {
		this->gui->get<tgui::ChildWindow>("chWindow")->close();
		this->on = false;
	}

	void setVisible(bool isVisible) {
		this->gui->get<tgui::ChildWindow>("chWindow")->setVisible(isVisible);
	}
};