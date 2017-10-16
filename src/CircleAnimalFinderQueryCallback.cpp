#include "CircleAnimalFinderQueryCallback.hpp"

CircleAnimalFinderQueryCallback::CircleAnimalFinderQueryCallback(PlayContext* playContext, int floor, float radius, b2Vec2 worldCenterOfBlast)  {
    mPlayContext = playContext;
    mFloor = floor;
    mRadius = radius;
    mWorldCenterOfBlast = worldCenterOfBlast;
}

std::vector<Animal*> CircleAnimalFinderQueryCallback::queryWorldAndGetAnimals() {
    b2AABB aabb;
    aabb.lowerBound = mWorldCenterOfBlast - b2Vec2(mRadius,mRadius);
    aabb.upperBound = mWorldCenterOfBlast + b2Vec2(mRadius,mRadius);
    
    this->mPlayContext->getWorld()->QueryAABB(this, aabb);
    
    return mAffectedAnimals;
}

#pragma mark - b2QueryCallback Override

bool CircleAnimalFinderQueryCallback::ReportFixture(b2Fixture* fixture) {
    if(fixture->IsSensor()) {
        return true; // skip and keep looking
    }
    
    // TODO: check for same floor
    
    PlayEntity* entity = NULL;
    if(fixture->GetUserData() != NULL){
        entity = (PlayEntity*)fixture->GetUserData();
    } else {
        entity = (PlayEntity*)fixture->GetBody()->GetUserData();
    }
    
    if(entity == NULL) {
        return true; // skip; and keep looking
    }
    
    Animal* animal = NULL;
    if(ClassChecker().isKindOfClass<Animal*>(entity) ==  false){
        return true; // skip; and keep looking
    }
    
    animal = dynamic_cast<Animal*>(entity);
    if (animal) {
        if (b2Distance(animal->getBody()->GetPosition(),mWorldCenterOfBlast) < mRadius) {
            this->mAffectedAnimals.push_back(animal);
        }
    }
    
    return true; // keep looking
}