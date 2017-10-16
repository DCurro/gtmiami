#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "PooledPlayEntity.hpp"

class Explosion: public PooledPlayEntity {
protected:
	float mRadius;
	float mBaseDamage;

	Animation* mAnimation;

	uint64_t performOnce;

public:
	Explosion();
	virtual ~Explosion();

	using PooledPlayEntity::setup;
	virtual void setup(PlayContext* playContext, int floor, float radius, float baseDamage);
	virtual void prepareForReuse();

	virtual void update(float timeStep);

	// PlayEntity

	virtual Animation* newBulletStruckAnimation();
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	//<Inspectable>

	virtual std::vector<ItemDescription> getDebugDescription();
	virtual std::string getDebugName();
    
private:
    void damageNearbyEnemies();
    void informNearbyAnimalsOfSounds();
};

#endif /* EXPLOSION_HPP_ */
