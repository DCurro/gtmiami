/*
 * KungFuPrimalWeapon.hpp
 *
 *  Created on: Dec 13, 2014
 *      Author: DCurro
 */

#ifndef KUNGFUPRIMALWEAPON_HPP_
#define KUNGFUPRIMALWEAPON_HPP_

#include "PrimalWeapon.hpp"

class KungFuPrimalWeapon : public PrimalWeapon {
protected:
	// Weapon
	virtual Animation* idleAnimation() override;
	virtual float cooldownTimeInSeconds() override;
	virtual ResourceManager::SoundResources actionSoundEffect() override;
	virtual void strikeWithAngle(float angle) override;
	virtual bool strikeAtEndOfUse() override;

	// <Inspectable>
	std::vector<ItemDescription> getDebugDescription();
	std::string getDebugName();

public:
	KungFuPrimalWeapon();
	virtual ~KungFuPrimalWeapon();
};

#endif /* KUNGFUPRIMALWEAPON_HPP_ */
