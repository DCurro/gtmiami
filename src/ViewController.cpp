#include "ViewController.hpp"

#include "PlayContext.hpp"

ViewController::ViewController(PlayContext* playContext) : mPlayContext(playContext) {
	mView = sfg::Window::Create();
	mIsSetup = false;
	mClicked = false;
}

ViewController::~ViewController() {
}

void ViewController::setupGUI() {
	mIsSetup = true;
	mView->GetSignal(  sfg::Window::OnMouseLeftPress ).Connect(std::bind(&ViewController::clickedInWindow, this));
}

bool ViewController::clickWasOnGUIWindow() {
	return mClicked;
}

void ViewController::clickedInWindow() {
	mClicked = true;
	std::cout << "gui click auto detected" << std::endl;
}

void ViewController::update(float timestep) {
	UNUSED(timestep);
}

void ViewController::handleEvent(sf::Event event){
	UNUSED(event);
}
