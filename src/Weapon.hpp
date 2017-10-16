/*
 * Weapon.hpp
 *
 *  Created on: Dec 9, 2014
 *      Author: DCurro
 */

#ifndef WEAPON_HPP_
#define WEAPON_HPP_

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>

#include "common.hpp"
#include "PooledPlayEntity.hpp"
#include "ResourceManager.hpp"
#include "Animal.hpp"
#include "ClassChecker.hpp"
#include "Common.hpp"

#include "PlayContext.hpp"

class Weapon : public PooledPlayEntity {
protected:
	virtual float rangeInMeters() = 0;
	virtual Animation* idleAnimation() = 0;
//	virtual Animation* activeAnimation() = 0;
	virtual float cooldownTimeInSeconds() = 0;
	//return ResourceManager::SoundResources::no_sound if there is no sound effect
	virtual ResourceManager::SoundResources actionSoundEffect() = 0;
	virtual void strikeWithAngle(float angle) = 0;
	virtual bool strikeAtEndOfUse() = 0;
    virtual float soundRadius() = 0;

	ACCESSOR(bool, IsHeld);
	ACCESSOR(bool, CanAttackWithWeapon);

	float mAccumulatedCooldownTime;

	bool mIsStrikingWithWeapon;
	bool mJustFinishedStrikingWithWeapon;
	float mStrikingAngle;
	virtual void strike(float angle);

public:
	Weapon();
	virtual ~Weapon();

	using PooledPlayEntity::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);

	virtual void update(float timeStep);
	virtual void draw(sf::RenderTarget& window);

	virtual void startStrikingWithWeapon(float angle);
	virtual void changeAngle(float angle);
	virtual void stopStrikingWithWeapon(float angle);

protected:
	// PlayEntity
	virtual Animation *newBulletStruckAnimation() override;
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;
    
private:
    virtual void informNearbyAnimalsOfSound();
};

#endif /* WEAPON_HPP_ */
