#include "BunnyMind.hpp"

#include <stdlib.h>

#include "PlayContext.hpp"
#include "DistrictManager.hpp"
#include "NavigationCell.hpp"
#include "Animal.hpp"

BunnyMind::BunnyMind() {
}

BunnyMind::~BunnyMind() {
}

void BunnyMind::setup(PlayContext* playContext, Animal* controllingAnimal) {
    Mind::setup(playContext, controllingAnimal);
    
    float animalDiameter = this->mAnimal->getBodyFixture()->GetShape()->m_radius*2;
    mRouteController.setup(playContext, controllingAnimal, animalDiameter);
    mRouteController.delegate = this;
}

void BunnyMind::update(float timeStep)
{
    Mind::update(timeStep);
    
    mRouteController.update(timeStep);
}

void BunnyMind::draw(sf::RenderTarget& window) {
    Mind::draw(window);
    
    this->mRouteController.draw(window);
}

void BunnyMind::hearSoundAtLocation(b2Vec2 soundLocation) {
    b2Vec2 vectorPointingAwayFromTheSoundSource = (this->mAnimal->getBody()->GetPosition() - soundLocation);
    vectorPointingAwayFromTheSoundSource.Normalize();
    
    float distanceToTravel = 10.0f;
    
    b2Vec2 destinationRelativeToBody = b2Vec2(vectorPointingAwayFromTheSoundSource.x*distanceToTravel, vectorPointingAwayFromTheSoundSource.y*distanceToTravel);
    
    b2Vec2 finalDestination = this->mAnimal->getBody()->GetPosition()+destinationRelativeToBody;
    
    mRouteController.setDestination(finalDestination);
}

#pragma mark - <RouteControllerDelegate>

void BunnyMind::routeController_noLongerOnRoute(RouteController* routeController) {
    UNUSED(routeController);
    
//    routeController->setDestination(b2Vec2(10.0f, 10.0f));
    std::cout << "bunny no longer on route" << std::endl;
}

void BunnyMind::routeController_routeBecameBlocked(RouteController* routeController) {
    UNUSED(routeController);
    
//    routeController->setDestination(b2Vec2(10.0f, 10.0f));
    std::cout << "bunny route became blocked" << std::endl;
}

void BunnyMind::routeController_directionVectorToNextDestination(b2Vec2 destinationVector) {
    this->mAnimal->applyMotion(destinationVector, 1.0f);
    this->mAnimal->applyLook(b2Atan2(destinationVector.y, destinationVector.x) * RADIANS_TO_DEGREES);
    ;
}

void BunnyMind::routeController_didReachDestination(RouteController* routeController) {
    UNUSED(routeController);
    std::cout << "bunny did reach destination" << std::endl;
}