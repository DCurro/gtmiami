#ifndef ROCKETLAUNCHER_HPP_
#define ROCKETLAUNCHER_HPP_

#include "ProjectileGun.hpp"

class PlayContext;
class Animation;

class RocketLauncher : public ProjectileGun {

protected:
	Animation* mIdleAnimation;

public:
	RocketLauncher();
	virtual ~RocketLauncher();

	using ProjectileGun::setup;
	void setup(PlayContext* playContext, int persistentReferenceId, int floor);
	virtual void prepareForReuse();

	//PlayEntity
	virtual Animation*newBulletStruckAnimation() override;
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

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

	virtual void strike(float angle);

	//Projectile Gun
	virtual PooledPlayEntity* newProjectile(int floor, b2Vec2 position, float angle) override;
	virtual float launchVelocity() override;

	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* ROCKETLAUNCHER_HPP_ */
