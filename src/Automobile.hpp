#ifndef AUTOMOBILE_HPP_
#define AUTOMOBILE_HPP_

#include <vector>
#include <set>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>

#include "PooledPlayEntity.hpp"
#include "common.hpp"
#include "Animation.hpp"

class multiset;

class PlayContext;
class Animation;
class DriverBody;

class Automobile : public PooledPlayEntity {
protected:
	//the max speed the vehicle is allowed to reach while accelerating
	virtual float maxForwardVelocity() = 0;
	//the max speed the vehicle is allowed to reach while decelerating
	virtual float maxReverseVelocity() = 0;
	//maximum force to apply when accelerating (pick-up)
	virtual float maxForwardForce() = 0;
	//maximum force to apply when using the breaks
	virtual float maxBreakingForce() = 0;
	//1.0f would imply pivoting, where 0.0f would mean not being able to turn
	virtual float turnCoefficient() = 0;
	//drifting
	virtual float maxImpulseBeforeDrifting() = 0;
	//ease of turning
	virtual float angularImpulseWhileInDrive() = 0;
	//ease of turning while breaks are applied
	virtual float angularImpulseWhileParked() = 0;
	//drag
	virtual float dragAndTireFrictionWhileInDriveCoefficient() = 0;
	//drag while breaks are applied
	virtual float dragAndTireFrictionWhileParkedCoefficient() = 0;

	//animations in different states
	virtual Animation* idleAnimation() = 0;
//	virtual Animation* forwardAnimation() = 0;
//	virtual Animation* reversingAnimation() = 0;

	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	ACCESSOR(b2Body*, StaticBody);

	GETTER(std::vector<b2Fixture*>*, PriorityOrderedDoorSensorFixtures);
	GETTER(std::vector<std::multiset<PlayEntity*>*>*, DoorExitLocationObstructionMultiSetList);

	//location of where the driver should be placed when exiting the vehicle
	// returns a new b2Vec2 if there is a reasonable location, which the user will have to delete when they are done using it;
	// returns NULL if there is no reasonable location
	virtual b2Vec2* preferredExitLocation();

	ACCESSOR(bool, IsParked);
	virtual float angularImpulse();
	virtual float dragAndTireFrictionCoefficient();


	static b2Vec2 getCircleFixtureWorldCentre(b2Body* body, b2Fixture* circleShapedFixture);

	DriverBody* mDriver;

	Animation* mCurrentAnimaton;

	float mGasPower;
	float mTurnPower;
	float mBreakPower;

	void clearDriverInput();

	float mLastSpeed;

	bool mIsStopped;
	bool mCanReverse;
	float mStoppedTimer;

	b2Vec2 getForwardVelocity();
	b2Vec2 getLateralVelocity();
	//returns force applied
	float updateDrive(float timeStep);
	void updateTurn(float timeStep);
	void updateFriction();

public:
	Automobile();
	virtual ~Automobile();

	using PooledPlayEntity::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);

    ACCESSOR(b2Fixture *, StaticBodyFixture);
    
	virtual void addDoorSensorFixtureAsNextMostPriority(b2Fixture* doorSensorFixture);

	//Driver API
	virtual void applyGas(float power);
	virtual void applyBreak();
	virtual void turnWheel(float turn);
	virtual bool setDriver(DriverBody* driver);
	virtual bool hasDriver();
	/* dropOffDriver
	 *
	 * The driver will attempt to exit the vehicle.  The drive may exit if the way isn't obstructed.
	 */
	virtual void dropOffDriver();
	virtual std::vector<b2Vec2> getDoorPositions();
};

#endif /* AUTOMOBILE_HPP_ */
