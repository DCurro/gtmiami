/*
 * Spirit.hpp
 *
 *  Created on: Dec 8, 2014
 *      Author: DCurro
 */

#ifndef SPIRIT_HPP_
#define SPIRIT_HPP_

#include "Person.hpp"

class Spirit: public Person {
protected:
	ACCESSOR(Animation*, IdleAnimation);

public:
	Spirit();
	virtual ~Spirit();

	using Person::setup;
	virtual void setup(PlayContext* playContext, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);

	//<Animal>
	virtual float motionSpeed() override;
	virtual float maxHealthPoints() override;
	virtual Animation* idleAnimation() override;
	virtual Animation* walkAnimation() override;
	virtual Animation* deadAnimation() override;
    virtual void hearWeaponSoundAtLocation(b2Vec2 location) override;

	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* SPIRIT_HPP_ */
