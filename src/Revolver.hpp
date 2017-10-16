#ifndef REVOLVER_HPP_
#define REVOLVER_HPP_

#include "BulletGun.hpp"

class PlayContext;
class Animation;

class Revolver : public BulletGun {
public:
	Revolver();
	virtual ~Revolver();

	using BulletGun::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);
	virtual void prepareForReuse();

protected:
	Animation* mAnimation;

	// Weapon
	virtual bool strikeAtEndOfUse() override;
    virtual float soundRadius() override;

	// Gun
	virtual int maxAmmoCount() override;
	virtual float cooldownTimeInSeconds() override;
	virtual float rangeInMeters() override;
	virtual Animation* idleAnimation() override;
	virtual bool isAutomaticWeapon() override;
	virtual ResourceManager::SoundResources actionSoundEffect() override;

	// BulletGun
	virtual float bulletCalibre() override;

	// <Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription();
	virtual std::string getDebugName();
};

#endif /* REVOLVER_HPP_ */
