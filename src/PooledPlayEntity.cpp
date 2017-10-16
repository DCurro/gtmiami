#include "PooledPlayEntity.hpp"
#include <iostream>
#include "PlayContext.hpp"

PooledPlayEntity::PooledPlayEntity() : PlayEntity() {

}

PooledPlayEntity::~PooledPlayEntity() {

}

void PooledPlayEntity::sendIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) {
	std::vector<selfPair> mapVec;
	for(auto receiver : mListeners){
		receiver.first->onReceivedIdentificationNumberHasBecomeInvalidNotification(oldID);
	}
}

void PooledPlayEntity::registerListener(IdentificationNumberChangedNotificationReceiver* receiver) {
	std::pair<IdentificationNumberChangedNotificationReceiver*,IdentificationNumberChangedNotificationReceiver*> pair(receiver,receiver);
	mListeners.insert(pair);
}

void PooledPlayEntity::unregisterListener(IdentificationNumberChangedNotificationReceiver* receiver) {
	if( mListeners.count(receiver) > 0 ) {
		mListeners.erase(receiver);
	}
}

void PooledPlayEntity::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
    PlayEntity::setup(playContext, floor);
    
    this->mPersistentIdNumber = persistentIdNumber;
}

void PooledPlayEntity::update(float timeStep) {
    PlayEntity::update(timeStep);
}

void PooledPlayEntity::prepareForReuse(){
	mIsStillPartOfGame = true;

	if(mBody){
		mPlayContext->getWorld()->DestroyBody(mBody);
		mBody=NULL;
	}

	mAngle = 0.0f;
	mScreenPosition = sf::Vector2f();
	mSprite = sf::Sprite();

	mListeners.clear();
}
