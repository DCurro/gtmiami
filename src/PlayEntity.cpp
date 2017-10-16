#include <math.h>
#include <limits>
#include <iostream>

#include "PlayEntity.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "GameObject.hpp"
#include "PooledPlayEntity.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "Camera.hpp"

PlayEntity::PlayEntity(){
	mPlayContext = NULL;
	mBody = NULL;
	mCurrentFloor = 0;
	mID = -1;
	mTeamID = -1;
	mAngle = 0.0f;
	mIsStillPartOfGame = true;
}

PlayEntity::~PlayEntity() {
	mPlayContext = NULL;
	mBody = NULL;
}

void PlayEntity::setup(PlayContext* playContext, int floor) {
	mPlayContext = playContext;
	mCurrentFloor = floor;
    
    this->lastPosition = b2Vec2(0,0);
    this->lastRotation = 0.0f;
}

void PlayEntity::update(float timeStep){
	for(unsigned int i=0; i<mBulletStrikeAnimations.size(); i++){
		Animation* bulletStrikeAnimation = mBulletStrikeAnimations.at(i);
		if(bulletStrikeAnimation->hasCompletedAnimation()) {
			delete mBulletStrikeAnimations.at(i);
			mBulletStrikeAnimations.erase(mBulletStrikeAnimations.begin()+i);
			mBulletStrikeSprites.erase(mBulletStrikeSprites.begin()+i);
			i--;
		}

	}
    
	for(unsigned int i=0; i<mBulletStrikeAnimations.size(); i++){
		Animation* bulletStrikeAnimation = mBulletStrikeAnimations.at(i);
		bulletStrikeAnimation->update(timeStep);

		mBulletStrikeSprites.at(i).setTexture(*bulletStrikeAnimation->getCurrentFrame(), true);
	}
    
    if (mBody) {
        if(this->getHasMoved()) {
//            std::cout << "I moved!" << std::endl;
        }

        this->lastPosition = this->getBody()->GetPosition();
        this->lastRotation = this->getBody()->GetAngle();
    }
}

void PlayEntity::draw(sf::RenderTarget& window) {
    if(mBody!=NULL){
        mSprite.setOrigin(sf::Vector2f(mSprite.getTextureRect().width/2, mSprite.getTextureRect().height/2));
        
        mScreenPosition.x = worldToScreen(mBody->GetPosition().x);
        mScreenPosition.y = worldToScreen(mBody->GetPosition().y);
        mAngle = mBody->GetAngle()*(360.0f/(2.0*PI_CONSTANT));
        
        mSprite.setPosition(mScreenPosition);
        mSprite.setRotation(mAngle);
    }
    
	window.draw(mSprite);

	for(sf::Sprite sprite : mBulletStrikeSprites) {
		window.draw(sprite);
	}
}

int PlayEntity::getTeamID(){
	return mTeamID;
}

bool PlayEntity::isOnFloor(int floor) {
	return (mCurrentFloor == floor);
}

bool PlayEntity::getHasMoved() {
    if (mBody == NULL) {
        return false;
    }
    
    b2Vec2 currentPosition = this->getBody()->GetPosition();
    float currentRotation = this->getBody()->GetAngle();
    
    if (currentPosition.x != lastPosition.x ||
        currentPosition.y != lastPosition.y ||
        currentRotation != lastRotation)
    {
        return true;
    }
    
    return false;
}

void PlayEntity::clearPhysicalMotion(){
	if(mBody){
		mBody->SetLinearVelocity(b2Vec2(0,0));
		mBody->SetAngularVelocity(0);
	}
}

void PlayEntity::strikeWithBulletAtWorldLocationWithNormalAngle(b2Vec2 worldLocation, float normalAngleInDegrees, float bulletCalibre) {
	this->struckByBulletConsequences(bulletCalibre);

	Animation* strikeAnimation = this->newBulletStruckAnimation();

	if(strikeAnimation != NULL){
		mBulletStrikeAnimations.push_back(strikeAnimation);

		sf::Sprite sprite;
		sprite.setTexture(*strikeAnimation->getCurrentFrame(), true);
		sf::Vector2f screenLocation( Camera::worldToScreen(sf::Vector2f(worldLocation.x, worldLocation.y)) );
		sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().width/2, sprite.getTextureRect().height/2));
		sprite.setPosition(screenLocation);
		sprite.setRotation(normalAngleInDegrees);
		mBulletStrikeSprites.push_back(sprite);
	}
}

void PlayEntity::strikeWithBlastForce(b2Vec2 blastForce, float damage) {
	this->struckByBlastConsequences(blastForce, damage);
}
