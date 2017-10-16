#include "GameObject.hpp"

#include "PooledPlayEntity.hpp"

GameObject::GameObject() {
}

GameObject::~GameObject() {
}

/** registerPooledPlayEntity
 *
 * registers a pooledPlayEntity for future reference and id validity checking
 */
void GameObject::registerPooledPlayEntity(PooledPlayEntity* entity) {
	mPooledPlayEntityIdMap.insert(std::pair<unsigned int, PooledPlayEntity*>(entity->getID(), entity));
	entity->registerListener(this);
}

// <IdentificaitonNumberChangedReceiver>
void GameObject::onReceivedIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) {
	if(mPooledPlayEntityIdMap.find(oldID) != mPooledPlayEntityIdMap.end()) {
		mPooledPlayEntityIdMap.erase(oldID);
	}
}

/** isValid
 *
 * checks if this object has entity registered, and returns true if
 * the current identification number matches the associated number found
 * at registration.
 */
bool GameObject::isValid(PooledPlayEntity* entity) {
	if(!entity) {
		return false;
	}

	bool hasMatchingId = false;

	if(mPooledPlayEntityIdMap.find(entity->getID()) != mPooledPlayEntityIdMap.end()) {
		if(mPooledPlayEntityIdMap.find(entity->getID())->first == mPooledPlayEntityIdMap.find(entity->getID())->second->getID()){
			hasMatchingId = true;
		}
	}

	return hasMatchingId;
}
