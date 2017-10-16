#ifndef BULLETGUN_HPP_
#define BULLETGUN_HPP_

#include "Gun.hpp"

class BulletGun : public Gun {
protected:
	virtual float bulletCalibre() = 0;

	// Gun
	virtual void strikeWithAngle(float angle) override;

public:
	BulletGun();
	virtual ~BulletGun();

	using Gun::setup;
    virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);
};

#endif /* BULLETGUN_HPP_ */
