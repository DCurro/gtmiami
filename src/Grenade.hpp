#ifndef GRENADE_HPP_
#define GRENADE_HPP_

#include "AuxiliaryWeapon.hpp"

class PlayContext;
class Animation;

class Grenade: public AuxiliaryWeapon {
protected:
	Animation* mIdleAnimation;

public:
	Grenade();
	virtual ~Grenade();

	using AuxiliaryWeapon::setup;
	void setup(PlayContext* playContext, int persistentReferenceId, int floor);
	virtual void prepareForReuse();

	// PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation*newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
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
	virtual ResourceManager::SoundResources actionSoundEffect() override;

	virtual void strike(float angle);

	// Projectile Gun
	virtual PooledPlayEntity* newProjectile(int floor, b2Vec2 position, float angle) override;
	virtual float launchVelocity() override;

	// <Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* GRENADE_HPP_ */
