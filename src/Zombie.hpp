

#ifndef __gtmiami__BadGuy__
#define __gtmiami__BadGuy__

#include <stdio.h>

#include "Person.hpp"

class Zombie : public Person {
private:
    Animation* mWalkingAnimation;
    Animation* mIdleAnimation;
    Animation* mDeadAnimation;
    
public:
    Zombie();
    virtual ~Zombie();
    
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

#endif /* defined(__gtmiami__BadGuy__) */
