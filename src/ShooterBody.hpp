#ifndef SHOOTERBODY_HPP_
#define SHOOTERBODY_HPP_

#include "common.hpp"
#include "Gun.hpp"

class ShooterBody {
	ACCESSOR(b2Fixture*, WeaponSensorFixture);
	GETTER_AND_DEFAULT_NULL(Gun*, HeldGun);
public:
	ShooterBody();
	virtual ~ShooterBody();

	virtual void dropGun();
	virtual void holdGun(Gun* gun);
	virtual bool isArmed();
	virtual void pullTheTrigger();

	virtual void closeToGun(Gun* gun) = 0;
	virtual void awayFromGun(Gun* gun) = 0;
};

#endif /* SHOOTERBODY_HPP_ */
