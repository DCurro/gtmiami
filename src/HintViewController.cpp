#include "HintViewController.hpp"

#include "PlayContext.hpp"
#include "Game.hpp"

HintViewController::HintViewController(PlayContext* playContext) : ViewController(playContext) {
}

HintViewController::~HintViewController() {
}

void HintViewController::setupGUI() {
	ViewController::setupGUI();

	int labelHeight = 40;

	mView->SetStyle(sfg::Window::Style::BACKGROUND);
	mView->SetRequisition(sf::Vector2f(SCREEN_WIDTH,labelHeight));

	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL);
	mLabel = sfg::Label::Create("Right-click on an object to inspect.  Press G to toggle the GUI on and off.");
	box->Pack(mLabel, true, true);
	mView->Add( box );
}

void HintViewController::setDescription(std::string descriptionString) {
	mLabel->SetText(sf::String(descriptionString));
}
