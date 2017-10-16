#ifndef TOWTRUCK_HPP_
#define TOWTRUCK_HPP_

#include <vector>
#include <set>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>

#include "Automobile.hpp"
#include "common.hpp"
#include "Animation.hpp"

class multiset;

class PlayContext;
class Animation;
class DriverBody;

class TowTruck : public Automobile {
protected:
	Animation* mIdleAnimation;

	//<Automobile>
	virtual float maxForwardVelocity() override;
	virtual float maxReverseVelocity() override;
	virtual float maxForwardForce() override;
	virtual float maxBreakingForce() override;
	virtual float turnCoefficient() override;
	virtual float maxImpulseBeforeDrifting() override;
	virtual float dragAndTireFrictionWhileInDriveCoefficient() override;
	virtual float dragAndTireFrictionWhileParkedCoefficient() override;
	virtual float angularImpulseWhileInDrive() override;
	virtual float angularImpulseWhileParked() override;

	virtual Animation* idleAnimation() override;

public:
	TowTruck();
	virtual ~TowTruck();

	using Automobile::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor, b2Body* body, int teamID);
	virtual void prepareForReuse();

	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* TOWTRUCK_HPP_ */
