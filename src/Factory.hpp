#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "BunnyDescriptor.hpp"

#include "Pool.hpp"
#include "Hero.hpp"
#include "Turret.hpp"
#include "Bunny.hpp"
#include "Spirit.hpp"
#include "Car.hpp"
#include "TowTruck.hpp"
#include "Revolver.hpp"
#include "Ak47.hpp"
#include "RocketLauncher.hpp"
#include "RocketProjectile.hpp"
#include "Grenade.hpp"
#include "GrenadeProjectile.hpp"
#include "Explosion.hpp"
#include "Zombie.hpp"

#include "PooledPlayEntityDescriptor.hpp"
#include "BunnyDescriptor.hpp"
#include "TurretDescriptor.hpp"
#include "ZombieDescriptor.hpp"
#include "SpiritDescriptor.hpp"
#include "CarDescriptor.hpp"
#include "TowTruckDescriptor.hpp"
#include "RevolverDescriptor.hpp"
#include "Ak47Descriptor.hpp"
#include "RocketLauncherDescriptor.hpp"
#include "GrenadeDescriptor.hpp"

class PlayContext;
class PooledPlayEntity;

#define POOLS(ADD_POOL) \
	ADD_POOL(Hero) \
	ADD_POOL(Turret) \
	ADD_POOL(Bunny) \
    ADD_POOL(Zombie) \
	ADD_POOL(Spirit) \
	ADD_POOL(Car) \
	ADD_POOL(TowTruck) \
	ADD_POOL(Revolver) \
	ADD_POOL(Ak47) \
	ADD_POOL(RocketLauncher) \
	ADD_POOL(RocketProjectile) \
	ADD_POOL(Grenade) \
	ADD_POOL(GrenadeProjectile) \
	ADD_POOL(Explosion)

#define MAKE_TYPE_POOL(TYPE)  \
		private: \
			Pool<TYPE*> m##TYPE##Pool;

#define MAKE_POOL_ELSEIF(TYPE) \
		else if(dynamic_cast <TYPE*>(entity) !=NULL){ \
			std::cout << "giving back a " << #TYPE << " to the pool" << std::endl;\
			m##TYPE##Pool.giveBack( dynamic_cast<TYPE*>(entity) ); \
		}

#define DEBUG_PRINT_POOLS(TYPE) \
		std::cout << "Pool " << #TYPE << " size(" << m##TYPE##Pool.size() << ") - contains(" << m##TYPE##Pool.contains() << ") leased(" << m##TYPE##Pool.leased() << ")" << std::endl;

class Factory {
private:
	POOLS(MAKE_TYPE_POOL)

public:
	Factory();
	virtual ~Factory();

    PooledPlayEntity *pooledPlayEntityForDescriptor(PlayContext* playContext,
                                                    PooledPlayEntityDescriptor *pooledPlayEntityDescriptor);
    
    Hero* createHero(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angle);
    Spirit* createSpirit(PlayContext* playContext, int floor, Mind* mind, b2Vec2 worldPosition, float angle);
	
    RocketProjectile* createRocketProjectile(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angle);
	GrenadeProjectile* createGrenadeProjectile(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angle);
	Explosion* createExplosion(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angle, float blastRadius, float baseDamage);

    
private:
    // descriptor creators
    Bunny* createBunnyFromDescriptor(PlayContext* playContext, BunnyDescriptor *descriptor);
    Turret* createTurretFromDescriptor(PlayContext* playContext, TurretDescriptor* descriptor);
    Zombie *createZombieFromDescriptor(PlayContext *playContext, ZombieDescriptor* descriptor);
    
    Car* createCarFromDescriptor(PlayContext* playContext, CarDescriptor *descriptor);
    TowTruck* createTowTruckFromDescriptor(PlayContext* playContext, TowTruckDescriptor *descriptor);
    
    Revolver* createRevolverFromDescriptor(PlayContext* playContext, RevolverDescriptor *descriptor);
    Ak47* createAk47FromDescriptor(PlayContext* playContext, Ak47Descriptor *descriptor);
    RocketLauncher* createRocketLauncherFromDescriptor(PlayContext* playContext, RocketLauncherDescriptor *descriptor);
    Grenade* createGrenadeFromDescriptor(PlayContext* playContext, GrenadeDescriptor *descriptor);
    
    // creators
    Turret* createTurret(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    Bunny* createBunny(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    Zombie *createZombie(PlayContext *playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    
    Car* createCar(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    TowTruck* createTowTruck(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    
    Revolver* createRevolver(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    Ak47* createAk47(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    RocketLauncher* createRocketLauncher(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);
    Grenade* createGrenade(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angle);

public:
	/** returnToPool
	 *
	 * returns a game entity to it's appropriate pool
	 */
	void returnToPool(PlayEntity* entity) {
		if(entity==NULL){

		}
		POOLS(MAKE_POOL_ELSEIF)
		else {
			std::cout << "ERROR: trying to pool a non pooled object" << std::endl;
		}
	}

	void debugPrint() {
		POOLS(DEBUG_PRINT_POOLS);
		std::cout << std::endl;
	}
};

#endif /* FACTORY_HPP_ */
