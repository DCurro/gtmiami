#ifndef GAMECONTEXT_HPP_
#define GAMECONTEXT_HPP_

#include "common.hpp"

class Game;
class ResourceManager;
class AudioManager;
class InputHandler;

class GameContext {
protected:
	ACCESSOR(Game*, Game);
	ACCESSOR(ResourceManager*, ResourceManager);
	ACCESSOR(AudioManager*, AudioManager);
	ACCESSOR(InputHandler*, InputHandler);

public:
	GameContext();
	virtual ~GameContext();
};

#endif /* GAMECONTEXT_HPP_ */
