#ifndef AK47_HPP_
#define AK47_HPP_

#include "BulletGun.hpp"

class PlayContext;
class Animation;

class Ak47: public BulletGun {
public:
	Ak47();
	virtual ~Ak47();

	using BulletGun::setup;
    void setup(PlayContext* playContext, int persistentIdNumber, int floor);
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
	virtual ResourceManager::SoundResources actionSoundEffect();

	// BulletGun
	virtual float bulletCalibre() override;

	// <Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription();
	virtual std::string getDebugName();
};

#endif /* AK47_HPP_ */
