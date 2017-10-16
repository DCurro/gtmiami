/*
 * Bunny.hpp
 *
 *  Created on: Dec 8, 2014
 *      Author: DCurro
 */

#ifndef BUNNY_HPP_
#define BUNNY_HPP_

#include "Animal.hpp"
#include "DriverBody.hpp"

class Bunny: public Animal, public DriverBody {
protected:
	ACCESSOR(Animation*, WalkingAnimation);
	ACCESSOR(Animation*, IdleAnimation);
	ACCESSOR(Animation*, DeadAnimation);

public:
	Bunny();
	virtual ~Bunny();

    virtual b2Body *getBody();
    
	using Animal::setup;
    virtual void setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);

	// Animal
	virtual float motionSpeed() override;
	virtual float maxHealthPoints() override;
	virtual Animation* idleAnimation() override;
	virtual Animation* walkAnimation() override;
	virtual Animation* deadAnimation() override;
    virtual void hearWeaponSoundAtLocation(b2Vec2 location) override;

	//<DriverBody>
	bool mIsPressingGasOrBreak;
	bool mIsSteering;

	virtual void drive() override;
	virtual void willBeginDriving() override;
	virtual void willEndDriving() override;

	virtual void requestDropOffDriver() override;
	virtual void requestApplyBreak() override;
	virtual void requestApplyGas(float gasPower) override;
	virtual void requestTurnWheel(float turnPower) override;

	// <Inspectable>
	std::vector<ItemDescription> getDebugDescription();
	std::string getDebugName();
};

#endif /* BUNNY_HPP_ */
