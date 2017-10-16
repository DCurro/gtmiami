#ifndef PLAYENTITY_HPP_
#define PLAYENTITY_HPP_

#include <map>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "GameObject.hpp"
#include "common.hpp"
#include "Inspectable.hpp"
#include "Pool.hpp"


class PlayContext;
class Animation;
class PooledPlayEntity;
class GameObject;

#define TEAMID_GOOD		1
#define TEAMID_BAD 		2
#define TEAMID_NEUTRAL 	3
#define TEAMID_UNSET	-1
#define TEAMID_GOODBULLET 4
#define TEAMID_NONINTERACTIVE 5

//TODO: maybe give play entities a health meter?

class PlayEntity : public GameObject {
public:
	enum class Team {
		GOOD, //can only affect bad and evil
		BAD, //can only affect good
		EVIL, //can only affect good or bad or evil
		WALL,
		BYSTANDER,
		NONPARTICIPATORY,
		INTERACTABLE
	};
    
protected:
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() = 0;
	//The sound effect to play when a bullet successfully lands on this entity
//	virtual ResourceManager::SoundResources struckByBulletSoundEffect() = 0;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) = 0;
	virtual void struckByBlastConsequences(b2Vec2 blastForce, float damage) = 0;

	int mTeamID;
	ACCESSOR(unsigned int, ID);

	ACCESSOR(PlayContext*, PlayContext);

	ACCESSOR(bool, IsStillPartOfGame);
	ACCESSOR(b2Body*, Body);
	ACCESSOR(b2Fixture*, BodyFixture);
	ACCESSOR(int, CurrentFloor);
	ACCESSOR(float, Angle);
	ACCESSOR(sf::Vector2f, ScreenPosition);
	ACCESSOR(sf::Sprite, Sprite);
    
    b2Vec2 lastPosition;
    float lastRotation = 0.0f;

public:
	PlayEntity();
	virtual ~PlayEntity();

	virtual void setup(PlayContext* playContext, int floor);

	virtual void update(float timeStep);
	virtual void draw(sf::RenderTarget& window);

	std::vector<Animation*> mBulletStrikeAnimations;
	std::vector<sf::Sprite> mBulletStrikeSprites;
	virtual void strikeWithBulletAtWorldLocationWithNormalAngle(b2Vec2 worldLocation, float normalAngleInDegrees, float bulletCalibre);
	virtual void strikeWithBlastForce(b2Vec2 blastForce, float damage);

	virtual void clearPhysicalMotion();

	virtual int getTeamID();

	virtual bool isOnFloor(int floor);
    
    virtual bool getHasMoved();
};

#endif /* PLAYENTITY_H_ */
