#ifndef __gtmiami__RouteFinderMind__
#define __gtmiami__RouteFinderMind__

#include "Mind.hpp"
#include "Common.hpp"
#include "RouteController.hpp"

class NavigationCell;

class BunnyMind : public Mind, public RouteControllerDelegate {
protected:
    RouteController mRouteController;
    
public:
    BunnyMind();
    virtual ~BunnyMind();
    
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
