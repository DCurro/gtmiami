#ifndef TIMESTEPSLIDERVIEWCONTROLLER_HPP_
#define TIMESTEPSLIDERVIEWCONTROLLER_HPP_

#include "ViewController.hpp"
#include "SFGUI/SFGUI.hpp"

class PlayContext;

class TimeStepSliderViewController: public ViewController {
protected:
	sfg::Adjustment::Ptr mAdjustment;

public:
	TimeStepSliderViewController(PlayContext* playContext);
	virtual ~TimeStepSliderViewController();

	virtual void setupGUI();

	void AdjustmentChanged();
};

#endif /* TIMESTEPSLIDERVIEWCONTROLLER_HPP_ */
