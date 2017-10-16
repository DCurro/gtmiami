#include "EntityManager.hpp"

#include "Factory.hpp"
#include "PlayContext.hpp"
#include "PooledPlayEntity.hpp"
#include "Camera.hpp"

EntityManager::EntityManager(PlayContext* playContext) : mPlayContext(playContext) {
}

EntityManager::~EntityManager() {
	auto iter = idToEntityMap.begin();
	while(iter != idToEntityMap.end()) {
		if(this->isValid(iter->second)){
			mPlayContext->getFactory()->returnToPool(iter->second);
			idToEntityMap.erase(iter->first);

			iter = idToEntityMap.begin();
		}
	}
}

void EntityManager::addEntity(PooledPlayEntity* playEntity) {
	this->registerPooledPlayEntity(playEntity);

	idEntityPairToAddOnNextUpdate.push_back( std::pair<int,PooledPlayEntity*>(playEntity->getID(),playEntity) );
}

PooledPlayEntity* EntityManager::getEntity(unsigned int id) {
	PooledPlayEntity* entity = NULL;

	auto pairFound = idToEntityMap.find(id);
	if(pairFound != idToEntityMap.end() && this->isValid(pairFound->second)) {
		entity = pairFound->second;
	}

	return entity;
}

bool EntityManager::doesPersistentEntityExists(int persistenceIdentifer) {
    for (auto pair : idToEntityMap ) {
        PooledPlayEntity* entity = pair.second;
        if(entity->getPersistentIdNumber() == persistenceIdentifer) {
            return true;
        }
    }
    
    return false;
}

void EntityManager::update(float timeStep) {
	//load the newly added entities
	for(auto idEntityPair : idEntityPairToAddOnNextUpdate) {
		idToEntityMap.insert( idEntityPair );
	}
	idEntityPairToAddOnNextUpdate.clear();

	//update entities, and build a list of entity ids to remove if they are no longer valid
	std::vector<int> idRemoveList;
	
	for (auto pair = idToEntityMap.begin(); pair != idToEntityMap.end(); ++pair) {
		if( isValid(pair->second) ){
			 if(pair->second->getIsStillPartOfGame()) {
				 pair->second->update(timeStep);
			 } else {
				 mPlayContext->getFactory()->returnToPool(pair->second);
			 }
		} else {
			idRemoveList.push_back(pair->first);
		}
	}
	
	//clear the invalid entities
	for(auto id : idRemoveList) {
		auto pairFound = idToEntityMap.find(id);
		if(pairFound != idToEntityMap.end()) {
			idToEntityMap.erase(id);
		}
	}
}

void EntityManager::draw(sf::RenderTarget& window) {
	for (auto iter = idToEntityMap.begin(); iter != idToEntityMap.end(); ++iter) {
		if( iter->second->isOnFloor(mPlayContext->getCamera()->getCurrentFloor()) ) {
			iter->second->draw(window);
		}
	}
}

// PlayEntity

Animation* EntityManager::newBulletStruckAnimation() {
	return NULL;
}

void EntityManager::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void EntityManager::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}
