#ifndef CircleAnimalFinderQueryCallback_HPP_
#define CircleAnimalFinderQueryCallback_HPP_

#include "GameObject.hpp"
#include "Common.hpp"
#include "PlayContext.hpp"
#include "Animal.hpp"
#include "ClassChecker.hpp"

class CircleAnimalFinderQueryCallback : public b2QueryCallback {
protected:
    PlayContext* mPlayContext;
    int mFloor;
    float mRadius;
    b2Vec2 mWorldCenterOfBlast;
    
    std::vector<Animal*> mAffectedAnimals;
    
public:
    CircleAnimalFinderQueryCallback(PlayContext* playContext, int floor, float radius, b2Vec2 worldCenterOfBlast);

    std::vector<Animal*> queryWorldAndGetAnimals();
    
    //b2QueryCallback
    bool ReportFixture(b2Fixture* fixture) override;
};

#endif
