#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include "PooledPlayEntity.hpp"

class Projectile: public PooledPlayEntity {
protected:
	virtual Animation* idleAnimation() = 0;
//	virtual ResourceManager::SoundResources intialSound() = 0;
//	virtual ResourceManager::SoundResources inMotionSound() = 0;
//	virtual ResourceManager::SoundResources landingSound() = 0;

public:
	Projectile();
	virtual ~Projectile();

	virtual void update(float timeStep);
};

#endif /* PROJECTILE_HPP_ */
