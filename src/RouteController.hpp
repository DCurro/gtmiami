#ifndef RouteNavigator_HPP_
#define RouteNavigator_HPP_

#include "GameObject.hpp"

#include <vector>

#include "Animal.hpp"
#include "PlayContext.hpp"
#include "NavigationCell.hpp"

class RouteControllerDelegate;

class RouteController : public GameObject {
public:
    RouteControllerDelegate* delegate;
    
    RouteController();
    virtual ~RouteController();
    
    virtual void setup(PlayContext* playContext, Animal* animal, float mAnimalDiameter);
    
    virtual void update(float timestep);
	virtual void draw(sf::RenderTarget& window);
    
    /** requests directions to a destination, which are
     *  returned by the RouteControllerDelegate methods
     */
    virtual void setDestination(b2Vec2 destinationVector);
    
protected:
    PlayContext *mPlayContext;
    
    Animal* mAnimal;
    float mAnimalDiameter;
    
    NavigationCell* mCurrentNavCell;
    NavigationCell* mDestinationCell;
    
    std::vector<NavigationCell*> mRoute;
    std::vector<NavigationCell*> mOriginalRoute; //the route before we knocked off navigated cells
    std::vector<NavigationCell*> mNextRoute;
    
    virtual bool isStillOnRoute(std::vector<NavigationCell*> route);

    virtual void debugDrawRoute(sf::RenderTarget& window);
};

class RouteControllerDelegate {
public:
    virtual void routeController_noLongerOnRoute(RouteController* routeController) = 0;
    virtual void routeController_routeBecameBlocked(RouteController* routeController) = 0;
    virtual void routeController_directionVectorToNextDestination(b2Vec2 destinationVector) = 0;
    virtual void routeController_didReachDestination(RouteController* routeController) = 0;
};

#endif
