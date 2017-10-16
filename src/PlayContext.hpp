#ifndef PLAYCONTEXT_HPP_
#define PLAYCONTEXT_HPP_

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class GameContext;
class PlayGameMode;
class InputHandler;
class Factory;
class EntityManager;
class Scoreboard;
class Camera;
class GUIManager;
class DistrictManager;
class GameTime;

#include "common.hpp"
#include "Box2DWorld.hpp"

class PlayContext {
	ACCESSOR(GameContext*, GameContext);
	ACCESSOR(PlayGameMode*, PlayGameMode);
	ACCESSOR(Box2DWorld*, World);
	ACCESSOR(Factory*, Factory);
	ACCESSOR(EntityManager*, EntityManager);
	ACCESSOR(Camera*, Camera);
	ACCESSOR(GUIManager*, GUIManager);
	ACCESSOR(DistrictManager*, DistrictManager);
	ACCESSOR(GameTime*, GameTime);
	ACCESSOR(sf::Shader*, Shader);
	ACCESSOR(bool, IsUsingShader);

	//TODO: have multiple light locations (the sun moving across the sky, street lights, ect.)
	ACCESSOR(sf::Vector3f, LightLocation);

public:
	PlayContext();
	virtual ~PlayContext();
};

#endif /* PLAYCONTEXT_HPP_ */
