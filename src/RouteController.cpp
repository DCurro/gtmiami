#include "RouteController.hpp"

#include "PlayContext.hpp"
#include "DistrictManager.hpp"
#include "District.hpp"
#include "Camera.hpp"
#include "GameContext.hpp"
#include "Game.hpp"

RouteController::RouteController() {

}

RouteController::~RouteController() {
    
}

void RouteController::setup(PlayContext* playContext, Animal* animal, float animalDiameter) {
    mPlayContext = playContext;
    mAnimal = animal;
    mAnimalDiameter = animalDiameter;
    mCurrentNavCell = NULL;
    mDestinationCell = NULL;
    delegate = NULL;
}

void RouteController::update(float timeStep)
{
    UNUSED(timeStep);
    
    if (mDestinationCell == NULL) {
        return;
    }
    
    if (mNextRoute.size() > 0) {
        mRoute = mNextRoute;
        mNextRoute.clear();
    }
    
    std::vector<NavigationCell*> currentNavCellVec;
    currentNavCellVec.push_back(mCurrentNavCell);
    
    if (this->isStillOnRoute(mOriginalRoute) == false &&
        this->isStillOnRoute(currentNavCellVec) == false)
    {
        this->mRoute.clear();
        
        if (delegate) {
            delegate->routeController_noLongerOnRoute(this);
        }
    }
    
    if (mRoute.size() > 0) {
        NavigationCell* destCell = mRoute.at(0);
        
        b2Vec2 startPosition = this->mAnimal->getBody()->GetPosition();
        b2Vec2 endPosition;
        
        b2Vec2 directionVector(0,0);
        
        float navCellSize = (District::METERES_PER_TILE/NAVIGATION_CELLS_PER_TILE_WIDE);
        int numberOfOccupiedTilesWide = (int)std::ceil(mAnimalDiameter/navCellSize);
        
        if (numberOfOccupiedTilesWide%2 == 0) { //even number so it fits best in corner
            endPosition = destCell->getBottomRightPoint();
            
            directionVector = destCell->getBottomRightPoint() - this->mAnimal->getBody()->GetPosition();
            directionVector.Normalize();
        } else {
            endPosition = destCell->getCenterPoint();
            
            directionVector = destCell->getCenterPoint() - this->mAnimal->getBody()->GetPosition();
            directionVector.Normalize();
        }
        
        float32 distance = b2Distance(startPosition, endPosition);
        
        bool shouldGiveDirections = true;
        
        if ( distance > 0.25 ) {
            if (startPosition.x>endPosition.x && startPosition.y>endPosition.y) {
                directionVector = b2Vec2(-1,-1);
                directionVector.Normalize();
            } else if (startPosition.x>endPosition.x) {
                directionVector = b2Vec2(-1,0);
                directionVector.Normalize();
            } else if (startPosition.y>endPosition.y) {
                directionVector = b2Vec2(0,-1);
                directionVector.Normalize();
            }
            
            if (destCell->isOccupiedByAnythingOtherThanEntity(this->mAnimal) == true) {
                this->mRoute.clear();
                if (delegate) {
                    delegate->routeController_routeBecameBlocked(this);
                }
            }
        } else {
            mCurrentNavCell = mRoute.at(0);
            if (mCurrentNavCell == mDestinationCell) {
                if (delegate) {
                    shouldGiveDirections = false;
                    delegate->routeController_didReachDestination(this);
                }
            }
            mRoute.erase(mRoute.begin());
        }
        
        if (delegate && shouldGiveDirections) {
            delegate->routeController_directionVectorToNextDestination(directionVector);
        }
    }
}

void RouteController::draw(sf::RenderTarget& window){
//    this->debugDrawRoute(window);
}

void RouteController::setDestination(b2Vec2 destinationVector) {
    
    b2Vec2 startVector = this->mAnimal->getBody()->GetPosition();
   
    float animalDiameter = this->mAnimal->getBodyFixture()->GetShape()->m_radius*2.0f;
    
    mNextRoute = this->mPlayContext->getDistrictManager()->plotRouteFromStartToEnd(startVector, destinationVector, this->mAnimal, animalDiameter);
    mOriginalRoute = mNextRoute;
    
    //we're already at these cells, so we don't need them for navigation
    
    float navCellSize = (District::METERES_PER_TILE/NAVIGATION_CELLS_PER_TILE_WIDE);
    int numberOfOccupiedTilesWide = (int)std::ceil(mAnimalDiameter/navCellSize);
    
    for (int i=0; i<numberOfOccupiedTilesWide; i++) {
        if (mNextRoute.size() > 0) {
            mNextRoute.erase(mNextRoute.begin());
        }
    }
    
    //the remaining cells are usable
    
    if (mNextRoute.size() > 0) {
        mDestinationCell = mNextRoute.at(mNextRoute.size()-1);
    } else {
        mDestinationCell = NULL;
    }
}

#pragma Private Helpers

bool RouteController::isStillOnRoute(std::vector<NavigationCell*> route) {
    for (NavigationCell* cell : route) {
        if (cell && cell->isOccupiedByEntity(this->mAnimal)) {
            return true;
        }
    }
    
    return false;
}

void RouteController::debugDrawRoute(sf::RenderTarget& window) {
    Camera *camera = mPlayContext->getCamera();
    for (int i=0; i<((int)mOriginalRoute.size())-2; i++) {
        sf::Vector2f start = camera->worldToScreen(sf::Vector2f(mOriginalRoute.at(i)->getCenterPoint().x,
                                                                mOriginalRoute.at(i)->getCenterPoint().y));
        sf::Vector2f end = camera->worldToScreen(sf::Vector2f(mOriginalRoute.at(i+1)->getCenterPoint().x,
                                                              mOriginalRoute.at(i+1)->getCenterPoint().y));
        
        sf::Vertex line[] =
        {
            sf::Vertex(start),
            sf::Vertex(end)
        };

        window.draw(line, 2, sf::Lines);
    }
}