#ifndef VIEWCONTROLLER_HPP_
#define VIEWCONTROLLER_HPP_

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Window.hpp>

#include "GameObject.hpp"
#include "common.hpp"

class PlayContext;

class ViewController : public GameObject {
protected:
	/* Automatically determine if the touch was within the contained View and it's contents.
	 * Does not work with button and button like GUI elements; these must set mClicked manually.
	 * */
	virtual void clickedInWindow();
	ACCESSOR(bool, Clicked);

	ACCESSOR(PlayContext*, PlayContext);
	GETTER(sfg::Window::Ptr, View);
	GETTER(bool, IsSetup);

public:
	/* No gui setup should occur in the constructor.  Use setupGUI. */
	ViewController(PlayContext* playContext);
	virtual ~ViewController();

	/* Setup gui elements here. */
	virtual void setupGUI();

	/*returns true if a mouse press occured within the contained View*/
	virtual bool clickWasOnGUIWindow();

	virtual void update(float timestep);
	virtual void handleEvent(sf::Event event);
};

#endif /* VIEWCONTROLLER_HPP_ */
