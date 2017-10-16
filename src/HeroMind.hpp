#ifndef HEROMIND_HPP_
#define HEROMIND_HPP_

#include "Mind.hpp"
#include "DriverMind.hpp"

class HeroMind: public Mind, public DriverMind {
protected:
	float mDesiredLookangleInDegrees;

public:
	HeroMind();
	virtual ~HeroMind();

	virtual void setup(PlayContext* playContext, Animal* controllingAnimal);

	virtual void update(float timeStep);

	virtual void drive() override;
};

#endif /* HEROMIND_HPP_ */
