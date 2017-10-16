#ifndef GUIMODE_HPP_
#define GUIMODE_HPP_

#include <string>

#include <SFML/Window.hpp>

#include "common.hpp"

class PlayContext;

class GUIMode {
protected:
	ACCESSOR(PlayContext*, PlayContext);
	GETTER(std::string, Description);

public:
	/* GUIMode
	 *
	 * No external modules should be modified from the constructor.  This is internal setup only.  If you
	 * need to setup outside modules, use initialize.
	 */
	GUIMode(PlayContext* playContext);
	virtual ~GUIMode();

	/* initialize
	 *
	 * Setup for modules external to this controller should occur here.  Placing them in
	 * the constructor may cause a conflict with the next GUIMode to be set.
	 */
	virtual void initialize() = 0;

	virtual void handleEvent(sf::Event event) = 0;

	/* destroy
	 *
	 * This mode is being removed from current mode state, and should replace any state necessary that
	 * it altered.
	 */
	virtual void destroy() = 0;

	/* description
	 *
	 * A helpful description of options that the user may perform while in the current state of this GUIMode.
	 */
	virtual std::string description() = 0;
};

#endif /* GUIMODE_HPP_ */
