#ifndef ENTITYMANAGER_HPP_
#define ENTITYMANAGER_HPP_

#include <map>
#include <set>
#include <string>

#include <SFML/Graphics.hpp>

#include "PlayEntity.hpp"
#include "common.hpp"
#include "Pool.hpp"

class PlayContext;
class PooledPlayEntity;


//TODO: make this game GameObject.  It probably doesn't need to be a PlayEntity
class EntityManager : public PlayEntity {
protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	ACCESSOR(PlayContext*, PlayContext);
	std::map<int, PooledPlayEntity*> idToEntityMap;
	std::vector< std::pair<int, PooledPlayEntity*> > idEntityPairToAddOnNextUpdate;

public:
	EntityManager(PlayContext* playContext);
	virtual ~EntityManager();

	virtual void addEntity(PooledPlayEntity* playEntity);
	virtual PooledPlayEntity* getEntity(unsigned int id);
    
    bool doesPersistentEntityExists(int persistenceIdentifer);

	virtual void update(float timeStep) override;
	virtual void draw(sf::RenderTarget& window) override;
};

#endif /* ENTITYMANAGER_HPP_ */
