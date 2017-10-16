#ifndef FLOORSELECTORVIEWCONTROLLER_HPP_
#define FLOORSELECTORVIEWCONTROLLER_HPP_

#include "ViewController.hpp"

class FloorSelectorViewController: public ViewController {
private:
	sfg::SpinButton::Ptr floorSpinner;
	int mCurrentFloor;

public:
	FloorSelectorViewController(PlayContext* playContext);
	virtual ~FloorSelectorViewController();

	virtual void setupGUI();
	void update(float timestep);

	void PositionFloorSpinnerChanged();
};

#endif /* FLOORSELECTORVIEWCONTROLLER_HPP_ */
