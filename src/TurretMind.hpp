#ifndef TurretMind_HPP_
#define TurretMind_HPP_

#include "Mind.hpp"

class TurretMind: public Mind {
protected:
	float mFaceAngle;

public:
	TurretMind();
	virtual ~TurretMind();

	virtual void setup(PlayContext* playContext, Animal* controllingAnimal);

	virtual void update(float timeStep);
};

#endif /* TurretMind_HPP_ */
