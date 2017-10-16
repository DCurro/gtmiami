#ifndef GUN_HPP_
#define GUN_HPP_

#include "Weapon.hpp"

//TODO: rename to firearm
class Gun: public Weapon {
protected:
	virtual int maxAmmoCount() = 0;
	virtual bool isAutomaticWeapon() = 0;

	GETTER(int, AmmoCount);

	std::vector<sf::RectangleShape> bulletCasts;

	void strike(float angle);

public:
	Gun();
	virtual ~Gun();

	using Weapon::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);

	virtual void draw(sf::RenderTarget& window);
};

#endif /* GUN_HPP_ */
