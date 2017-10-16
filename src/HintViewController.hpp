#ifndef HINTVIEWCONTROLLER_HPP_
#define HINTVIEWCONTROLLER_HPP_

#include <string>
#include <SFGUI/Widgets.hpp>

#include "ViewController.hpp"
#include "common.hpp"

class PlayContext;

class HintViewController : public ViewController {
private:
	std::string mDescriptionString;
	sfg::Label::Ptr mLabel;

public:
	HintViewController(PlayContext* playContext);
	virtual ~HintViewController();

	virtual void setupGUI();

	void setDescription(std::string descriptionString);
};

#endif /* HINTVIEWCONTROLLER_HPP_ */
