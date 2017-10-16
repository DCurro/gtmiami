#include "FloorSelectorViewController.hpp"

#include "District.hpp"
#include "Camera.hpp"
#include "PlayContext.hpp"

FloorSelectorViewController::FloorSelectorViewController(PlayContext* playContext) : ViewController(playContext) {
	mCurrentFloor = 0;
}

void FloorSelectorViewController::setupGUI() {
	ViewController::setupGUI();

	mView->SetStyle(sfg::Window::Style::BACKGROUND);
	mView->SetRequisition(sf::Vector2f(100,60));

	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL);
	sfg::Label::Ptr label = sfg::Label::Create("floor");
	sfg::Separator::Ptr separator = sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL);
	floorSpinner = sfg::SpinButton::Create(0,FLOORS_PER_DISTRICT-1,1);
	floorSpinner->SetDigits(0);
	floorSpinner->GetSignal( sfg::SpinButton::OnValueChanged ).Connect( std::bind( &FloorSelectorViewController::PositionFloorSpinnerChanged, this ) );

	box->Pack(label, true, true);
	box->Pack(separator, true, true);
	box->Pack(floorSpinner, true, true);
	mView->Add( box );
}

FloorSelectorViewController::~FloorSelectorViewController() {
}

void FloorSelectorViewController::update(float timestep) {
	ViewController::update(timestep);

}

void FloorSelectorViewController::PositionFloorSpinnerChanged(){
	mCurrentFloor = floorSpinner->GetValue();
	mPlayContext->getCamera()->setCurrentFloor(mCurrentFloor);
}
