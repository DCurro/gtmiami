#ifndef PLAYGAMEMODE_HPP_
#define PLAYGAMEMODE_HPP_

#include "GameMode.hpp"
#include <chrono>
#include <SFML/Graphics.hpp>

class PlayContext;
class Box2DDebugDraw;
class AudioManager;
class ActionTimer;
class Stairwell;

class PlayGameMode : public GameMode {
protected:
	ACCESSOR(PlayContext*, PlayContext);
	ACCESSOR(float, TimeStep);

	ActionTimer* mActionTimer;

	Stairwell* mStairwell;
	Stairwell* mStairwell2;

public:
	PlayGameMode(GameContext* gameContext);
	virtual ~PlayGameMode();

	AudioManager* audioManager;
	float fpsUpdateTimer;
	sf::Text fpsDisplay;
	Box2DDebugDraw * debugDrawer;

	virtual void initialize() override;
	virtual void handleEvent(sf::Event event) override;
	virtual void update(float timeStep) override;
	virtual void destroy() override;
	virtual void willDisappear() override;
	virtual void willAppear() override;

	void onActionTimerRing();
};

#endif /* PLAYGAMEMODE_HPP_ */
