#ifndef TURRET_HPP_
#define TURRET_HPP_

#include "Person.hpp"
#include "DriverBody.hpp"
#include "Hero.hpp"

class Turret : public Person {
private:
	Animation* mIdleAnimation;
	Animation* mDeadAnimation;

public:
	Turret();
	virtual ~Turret();

	using Person::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);

	void setTargetEntity(PooledPlayEntity* targetEntity);

	//PlayEntity
	Animation* newBulletStruckAnimation() override;

	//Animal
	virtual float motionSpeed() override;
	virtual float maxHealthPoints() override;
	virtual Animation* idleAnimation() override;
	virtual Animation* walkAnimation() override;
	virtual Animation* deadAnimation() override;
    virtual void hearWeaponSoundAtLocation(b2Vec2 location) override;
    
	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* BADGUY_HPP_ */
