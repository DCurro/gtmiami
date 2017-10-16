#ifndef ANIMAL_HPP_
#define ANIMAL_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>

#include <set>

#include "PooledPlayEntity.hpp"

class Automobile;
class PlayContext;
class Mind;
class Animation;
class Weapon;
class AuxiliaryWeapon;
class Gun;

class Animal : public PooledPlayEntity {
public:
	//the max speed that the person will move at in meters/second
	virtual float motionSpeed() = 0;
	virtual float maxHealthPoints() = 0;
	//animations in different states
	virtual Animation* idleAnimation() = 0;
	virtual Animation* walkAnimation() = 0;
//	virtual Animation* dyingAnimation() = 0;
	virtual Animation* deadAnimation() = 0;
//	virtual Animation* ridingMotorcycleAnimation() = 0; //move this into a subclass or interface
    virtual void hearWeaponSoundAtLocation(b2Vec2 location) = 0;

protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

    Mind* mMind;

	ACCESSOR(b2Fixture*, VisionSensorFixture);
	GETTER(std::set<PlayEntity*>*, VisionSet);

	float mHealthPoints;

	Animation* mCurrentAnimaton;

	b2Vec2 mMotionDirection;
	float mMotionPower;
	float mLookAngle;

	bool mPerformedSwitchWeapon;
	bool mPerformedBeginAttack;
	bool mPerformedEndAttack;

	ACCESSOR(b2Fixture*, BodyFixture);
	Weapon* mCurrentWeapon;
	Gun* mGun;
	AuxiliaryWeapon* mAuxiliaryWeapon;

	GETTER(std::multiset<Automobile*>*, LocalVehiclesMultiSet);
	GETTER(std::vector<Weapon*>*, WeaponsInPickupRange);

public:
	Animal();
	virtual ~Animal();

    virtual void setup(PlayContext* playContext, int floor); // do not use
	using PooledPlayEntity::setup;
    virtual void setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);
    virtual void draw(sf::RenderTarget& window);

	void pickUpClosestWeapon();

	//Body Driver API
	//direction is a normalized vector (length 1), power is a value between 0.0 and 1.0
	virtual void applyMotion(b2Vec2 direction, float power);
	//angle in degrees
	virtual void applyLook(float angle);
	//angle in degrees
	virtual void applyBeginAttack(float angleInDegrees);
	virtual void applyEndAttack(float angleInDegrees);
	virtual void applySwitchWeapon();
	virtual void applyTalk(b2Vec2 direction);
	ACCESSOR(bool, IsDriving);
	virtual void applyTryDriveClosestVehicle();
    
    // TODO: add to setup method
	virtual void setMind(Mind* mind);

};

#endif /* ANIMAL_HPP_ */
