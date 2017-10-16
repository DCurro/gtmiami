#include "Gun.hpp"

Gun::Gun() {
}

Gun::~Gun() {
}

void Gun::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
	Weapon::setup(playContext, persistentIdNumber, floor);

	mAmmoCount = this->maxAmmoCount();

	this->bulletCasts.clear();
}

void Gun::strike(float angle) {
	if(mAmmoCount <= 0) {
		return;
	}

	if(!this->isAutomaticWeapon()){
		if(mCanAttackWithWeapon == false) {
			return;
		}
		mCanAttackWithWeapon = false;
	}

	if(mAccumulatedCooldownTime < this->cooldownTimeInSeconds()) {
		return;
	}

	mAmmoCount = mAmmoCount-1;

	Weapon::strike(angle);
}

void Gun::draw(sf::RenderTarget& window) {
	Weapon::draw(window);

	for(sf::RectangleShape line : bulletCasts) {
		window.draw(line);
	}
	bulletCasts.clear();
}

