#ifndef GAMEMODE_HPP_
#define GAMEMODE_HPP_

class GameContext;

#include <SFML/Window.hpp>

#include "common.hpp"

class GameMode {
protected:

	ACCESSOR(GameContext*, GameContext);

public:
	GameMode(GameContext* gameContext);
	virtual ~GameMode();

	/** initialize
	 *
	 *	called when mode comes into use.
	 */
	virtual void initialize() = 0;

	virtual void handleEvent(sf::Event event) = 0;

	virtual void update(float timeStep) = 0;

	/** destroy
	 *
	 *  called when mode stops being used.
	 */
	virtual void destroy() = 0;

	/** willAppear
	 *
	 * This game mode is about to become active.
	 */
	virtual void willAppear() = 0;

	/** willDisappear
	 *
	 * This game mode is about to stop being active.
	 */
	virtual void willDisappear() = 0;
};

#endif /* GAMEMODE_HPP_ */
