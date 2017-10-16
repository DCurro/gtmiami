#ifndef GAME_HPP_
#define GAME_HPP_

#include <stack>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/SFGUI.hpp>

#include "common.hpp"
class GameContext;
class GameMode;

#define SCREEN_WIDTH 1366.0f
#define SCREEN_HEIGHT 768.0f

class Game {
protected:
	ACCESSOR(GameContext*, GameContext);
	GETTER(sf::RenderWindow*, Window);
	GETTER(sf::RenderTexture*, RenderTexture);
	ACCESSOR(sfg::SFGUI*, SFGUI);

	std::stack<GameMode*> mGameModeStack;

	GameMode* mGameModeToPush;
	bool mShouldPopGameMode;
	GameMode* mGameModeToReset;

public:
	Game();
	virtual ~Game();

	virtual void startWithTimeStep(float timeStep);

	virtual void update(float timeStep);

	virtual void requestPushGameMode(GameMode* gameMode);
	virtual void requestPopGameMode();
	virtual void requestResetGameMode(GameMode* gameMode);

private:
	virtual void pushGameMode(GameMode* gameMode);
	virtual void popGameMode();
	virtual void resetGameMode(GameMode* gameMode);
};

#endif /* GAME_HPP_ */
