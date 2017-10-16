#ifndef BadGuyMind_HPP_
#define BadGuyMind_HPP_

#include "GameObject.hpp"
#include "Mind.hpp"
#include "RouteController.hpp"
#include "Common.hpp"

class ZombieMind : public Mind, public RouteControllerDelegate {
protected:
    b2Vec2 mDesiredDestination;
    RouteController mRouteController;
    
public:
    ZombieMind();
    virtual ~ZombieMind();
    
    virtual void setup(PlayContext* playContext, Animal* controllingAnimal);
    
    virtual void update(float timeStep);
    virtual void draw(sf::RenderTarget& window);
    
    virtual void hearSoundAtLocation(b2Vec2 soundLocation) override;
    
public: //Abstract methods
    virtual void routeController_noLongerOnRoute(RouteController* routeController) override;
    virtual void routeController_routeBecameBlocked(RouteController* routeController) override;
    virtual void routeController_directionVectorToNextDestination(b2Vec2 destinationVector) override;
    virtual void routeController_didReachDestination(RouteController* routeController) override;

};

#endif
