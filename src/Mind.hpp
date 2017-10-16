#ifndef MIND_HPP_
#define MIND_HPP_

#include "PlayEntity.hpp"

class Animal;

class Mind : public PlayEntity {
protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	GETTER(Animal*, Animal);

public:
	Mind();
	virtual ~Mind();

	using PlayEntity::setup;
	virtual void setup(PlayContext* playContext, Animal* controllingAnimal);

	virtual void update(float timeStep);
    
	virtual void setTargetEntity(PooledPlayEntity* targetEntity);
	virtual PooledPlayEntity* getTargetEntity();
    
    virtual void hearSoundAtLocation(b2Vec2 soundLocation);
    
    virtual b2Body* getBody();

protected:
	PooledPlayEntity* mTargetEntity;
};

#endif /* MIND_HPP_ */
