#include "ZombieMind.hpp"

ZombieMind::ZombieMind() {
    
}

ZombieMind::~ZombieMind() {
    
}

void ZombieMind::setup(PlayContext* playContext, Animal* controllingAnimal) {
    Mind::setup(playContext, controllingAnimal);
    
    float animalDiameter = this->mAnimal->getBodyFixture()->GetShape()->m_radius*2;
    mRouteController.setup(playContext, controllingAnimal, animalDiameter);
    mRouteController.delegate = this;
}

void ZombieMind::update(float timeStep)
{
    Mind::update(timeStep);
    
    mRouteController.update(timeStep);
}

void ZombieMind::draw(sf::RenderTarget& window) {
    Mind::draw(window);
    
    this->mRouteController.draw(window);
}

void ZombieMind::hearSoundAtLocation(b2Vec2 soundLocation) {
    mDesiredDestination = soundLocation;
    
    mRouteController.setDestination(soundLocation);
}

#pragma mark - <RouteControllerDelegate>

void ZombieMind::routeController_noLongerOnRoute(RouteController* routeController) {
    routeController->setDestination(mDesiredDestination);
    std::cout << "zombie no longer on route" << std::endl;
}

void ZombieMind::routeController_routeBecameBlocked(RouteController* routeController) {
    routeController->setDestination(mDesiredDestination);
    std::cout << "zombie route became blocked" << std::endl;
}

void ZombieMind::routeController_directionVectorToNextDestination(b2Vec2 destinationVector) {
    this->mAnimal->applyMotion(destinationVector, 1.0f);
    this->mAnimal->applyLook(b2Atan2(destinationVector.y, destinationVector.x) * RADIANS_TO_DEGREES);
    ;
}

void ZombieMind::routeController_didReachDestination(RouteController* routeController) {
    UNUSED(routeController);
    
    std::cout << "zombie did reach destination" << std::endl;
}