#include "RoutePlotter.hpp"

#include <algorithm>
#include "District.hpp"

#pragma mark - Public

std::vector<NavigationCell *> RoutePlotter::plotRouteBetweenNavCells(NavigationCell * start,
                                                                     NavigationCell * trueEnd,
                                                                     Animal* navigatorAnimal,
                                                                     float navigatorDiameter)
{
    NavigationCell* destinationCell = NULL;
    
    this->clearData();
    
    this->mQueue.push(start);
    this->markNavCellAsVisited(start, BackTrackEnum::NONE);
    
    bool didFindRouteClosestToEnd = false;
    
    int pathLength = 0;
    
    float shortestDistanceToTrueEnd = MAXFLOAT;
    
    while (mQueue.size() > 0 && mQueue.front()) {
        NavigationCell *currentCell = mQueue.front();
        mQueue.pop();
        
        if (currentCell == trueEnd) {
            destinationCell = trueEnd;
            didFindRouteClosestToEnd = true;
            break;
        }
        
        float navCellSize = (District::METERES_PER_TILE/NAVIGATION_CELLS_PER_TILE_WIDE);
        int numberOfOccupiedTilesWide = (int)std::ceil(navigatorDiameter/navCellSize);
        
        float distanceToTrueEnd = 0;
        
        if (numberOfOccupiedTilesWide%2 == 0) {
            distanceToTrueEnd = (currentCell->getBottomRightPoint() - trueEnd->getBottomRightPoint()).Length();
        } else {
            distanceToTrueEnd = (currentCell->getCenterPoint() - trueEnd->getCenterPoint()).Length();
        }
        
        if (distanceToTrueEnd < shortestDistanceToTrueEnd) {
            shortestDistanceToTrueEnd = distanceToTrueEnd;
            destinationCell = currentCell;
            didFindRouteClosestToEnd = true;
        }
        
        pathLength++;
        if (pathLength > MAX_PATH_LENGTH) {
            break;
        }
        
//        NavigationCell * northEastCell = currentCell->getNorthEastCell();
//        NavigationCell * northWestCell = currentCell->getNorthWestCell();
//        NavigationCell * southEastCell = currentCell->getSouthEastCell();
//        NavigationCell * southWestCell = currentCell->getSouthWestCell();
        
        NavigationCell * eastCell = currentCell->getEastCell();
        NavigationCell * westCell = currentCell->getWestCell();
        NavigationCell * northCell = currentCell->getNorthCell();
        NavigationCell * southCell = currentCell->getSouthCell();
        
        // TODO: randomize this pushing so that we get different paths
        
//        std::cout << navigatorAnimal->getDebugName() << " number of occupied tiles wide " << numberOfOccupiedTilesWide << std::endl;
        
        if (numberOfOccupiedTilesWide%2 == 0) { //even number so it fits best in corner
//            if (northEastCell
//                && this->isNavCellVisited(northEastCell)==false
//                && areNavCellsOccupiedFromBottomRightCorner(northEastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH_EAST) == false)
//            {
//                this->mQueue.push(northEastCell);
//                this->markNavCellAsVisited(northEastCell, BackTrackEnum::SOUTH_WEST);
//            }
//            
//            if (southEastCell
//                && this->isNavCellVisited(southEastCell)==false
//                && areNavCellsOccupiedFromBottomRightCorner(southEastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH_EAST) == false)
//            {
//                this->mQueue.push(southEastCell);
//                this->markNavCellAsVisited(southEastCell, BackTrackEnum::NORTH_WEST);
//            }
//            
//            if (northWestCell
//                && this->isNavCellVisited(northWestCell)==false
//                && areNavCellsOccupiedFromBottomRightCorner(northWestCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH_WEST) == false)
//            {
//                this->mQueue.push(northWestCell);
//                this->markNavCellAsVisited(northWestCell, BackTrackEnum::SOUTH_EAST);
//            }
//            
//            if (southWestCell
//                && this->isNavCellVisited(southWestCell)==false
//                && areNavCellsOccupiedFromBottomRightCorner(southWestCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH_WEST) == false)
//            {
//                this->mQueue.push(southWestCell);
//                this->markNavCellAsVisited(southWestCell, BackTrackEnum::NORTH_EAST);
//            }
            
            if (eastCell
                && this->isNavCellVisited(eastCell)==false
                && areNavCellsOccupiedFromBottomRightCorner(eastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::EAST) == false)
            {
                this->mQueue.push(eastCell);
                this->markNavCellAsVisited(eastCell, BackTrackEnum::WEST);
            }
            
            if (westCell!=NULL
                && this->isNavCellVisited(westCell)==false
                && areNavCellsOccupiedFromBottomRightCorner(westCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::WEST) == false)
            {
                this->mQueue.push(westCell);
                this->markNavCellAsVisited(westCell, BackTrackEnum::EAST);
            }
            
            if (northCell!=NULL
                && this->isNavCellVisited(northCell)==false
                && areNavCellsOccupiedFromBottomRightCorner(northCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH) == false)
            {
                this->mQueue.push(northCell);
                this->markNavCellAsVisited(northCell, BackTrackEnum::SOUTH);
            }
            
            if (southCell!=NULL
                && this->isNavCellVisited(southCell)==false
                && areNavCellsOccupiedFromBottomRightCorner(southCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH) == false)
            {
                this->mQueue.push(southCell);
                this->markNavCellAsVisited(southCell, BackTrackEnum::NORTH);
            }

            
        } else { //odd number so it fits best at center
//            if (northEastCell
//                && this->isNavCellVisited(northEastCell)==false
//                && areNavCellsOccupiedFromCenter(northEastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH_EAST) == false)
//            {
//                this->mQueue.push(northEastCell);
//                this->markNavCellAsVisited(northEastCell, BackTrackEnum::SOUTH_WEST);
//            }
//            
//            if (southEastCell
//                && this->isNavCellVisited(southEastCell)==false
//                && areNavCellsOccupiedFromCenter(southEastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH_EAST) == false)
//            {
//                this->mQueue.push(southEastCell);
//                this->markNavCellAsVisited(southEastCell, BackTrackEnum::NORTH_WEST);
//            }
//            
//            if (northWestCell
//                && this->isNavCellVisited(northWestCell)==false
//                && areNavCellsOccupiedFromCenter(northWestCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH_WEST) == false)
//            {
//                this->mQueue.push(northWestCell);
//                this->markNavCellAsVisited(northWestCell, BackTrackEnum::SOUTH_EAST);
//            }
//            
//            if (southWestCell
//                && this->isNavCellVisited(southWestCell)==false
//                && areNavCellsOccupiedFromCenter(southWestCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH_WEST) == false)
//            {
//                this->mQueue.push(southWestCell);
//                this->markNavCellAsVisited(southWestCell, BackTrackEnum::NORTH_EAST);
//            }
            
            if (eastCell
                && this->isNavCellVisited(eastCell)==false
                && areNavCellsOccupiedFromCenter(eastCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::EAST) == false)
            {
                this->mQueue.push(eastCell);
                this->markNavCellAsVisited(eastCell, BackTrackEnum::WEST);
            }
            
            if (westCell!=NULL
                && this->isNavCellVisited(westCell)==false
                && areNavCellsOccupiedFromCenter(westCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::WEST) == false)
            {
                this->mQueue.push(westCell);
                this->markNavCellAsVisited(westCell, BackTrackEnum::EAST);
            }
            
            if (northCell!=NULL
                && this->isNavCellVisited(northCell)==false
                && areNavCellsOccupiedFromCenter(northCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::NORTH) == false)
            {
                this->mQueue.push(northCell);
                this->markNavCellAsVisited(northCell, BackTrackEnum::SOUTH);
            }
            
            if (southCell!=NULL
                && this->isNavCellVisited(southCell)==false
                && areNavCellsOccupiedFromCenter(southCell, navigatorAnimal, navigatorDiameter/2.0f, BackTrackEnum::SOUTH) == false)
            {
                this->mQueue.push(southCell);
                this->markNavCellAsVisited(southCell, BackTrackEnum::NORTH);
            }
        }
    }
    
    std::vector<NavigationCell *> pathFromStartToEnd;
    
    if (didFindRouteClosestToEnd == true)
    {
        std::vector<NavigationCell *> backwardsPath;
        
        NavigationCell *currentCell = destinationCell;
        
        while (currentCell && currentCell != start) {
            backwardsPath.push_back(currentCell);
            
            BackTrackEnum cellEnteredFromDirection = this->mVisitedNavCells.find(currentCell)->second;
            if (currentCell->getEastCell() && cellEnteredFromDirection == BackTrackEnum::EAST) {
                currentCell = currentCell->getEastCell();
                
            } else if (currentCell->getWestCell() && cellEnteredFromDirection == BackTrackEnum::WEST) {
                currentCell = currentCell->getWestCell();
                
            } else if (currentCell->getNorthCell() && cellEnteredFromDirection == BackTrackEnum::NORTH) {
                currentCell = currentCell->getNorthCell();
                
            } else if (currentCell->getSouthCell() && cellEnteredFromDirection == BackTrackEnum::SOUTH) {
                currentCell = currentCell->getSouthCell();
                
            } else {
                break;
            }
        }
        
        backwardsPath.push_back(start);
        
        pathFromStartToEnd = backwardsPath;
        std::reverse(pathFromStartToEnd.begin(), pathFromStartToEnd.end());
    }
    
    return pathFromStartToEnd;
}

#pragma mark - Private Helpers

bool RoutePlotter::isNavCellVisited(NavigationCell *navCell)
{
    return (this->mVisitedNavCells.find(navCell) != mVisitedNavCells.end());
}

void RoutePlotter::markNavCellAsVisited(NavigationCell *navCell, BackTrackEnum backTrackDirection)
{
    std::pair<NavigationCell *, BackTrackEnum> pair = std::pair<NavigationCell *, BackTrackEnum>(navCell, backTrackDirection);
    
    this->mVisitedNavCells.insert(pair);
}

void RoutePlotter::clearData() {
    this->mVisitedNavCells = std::map<NavigationCell *,BackTrackEnum>();
    this->mQueue = std::queue<NavigationCell *>();
}

bool RoutePlotter::areNavCellsOccupiedFromCenter(NavigationCell* centerCell,
                                                 Animal* navigatorAnimal,
                                                 float radius,
                                                 BackTrackEnum direction)
{
    if (centerCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal) == true) {
        return true;
    }
    
    std::queue<NavigationCell*> cells;
//    std::map<NavigationCell*,NavigationCell*> visitedCells; // TODO: make this more efficient by making cells visited
    
    float halfCollisionCellSize = District::METERES_PER_TILE / NAVIGATION_CELLS_PER_TILE_WIDE;
    
    for (NavigationCell* currentCell=centerCell;
         b2Distance(centerCell->getCenterPoint(), currentCell->getCenterPoint())+halfCollisionCellSize < radius;)
    {
        NavigationCell* eastCell = currentCell->getEastCell();
        NavigationCell* westCell = currentCell->getWestCell();
        NavigationCell* northCell = currentCell->getNorthCell();
        NavigationCell* southCell = currentCell->getSouthCell();
        
        NavigationCell* northEastCell = currentCell->getNorthEastCell();
        NavigationCell* northWestCell = currentCell->getNorthWestCell();
        NavigationCell* southEastCell = currentCell->getSouthEastCell();
        NavigationCell* southWestCell = currentCell->getSouthWestCell();
        
        switch (direction) {
            case BackTrackEnum::NORTH_EAST: {
                if (!northEastCell
                    || !northCell
                    || !eastCell)
                {
                    return true;
                }
                
                if (northEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
            
            case BackTrackEnum::NORTH_WEST: {
                if (!northWestCell
                    || !northCell
                    || !westCell)
                {
                    return true;
                }
                
                if (northWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || westCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::SOUTH_EAST: {
                if (!southEastCell
                    || !eastCell
                    || !southCell)
                {
                    return true;
                }
                
                if (southEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::SOUTH_WEST: {
                if (!southWestCell
                    || !westCell
                    || !southCell)
                {
                    return true;
                }
                
                if (southWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || westCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::WEST: {
                if (!westCell
                    || !northWestCell
                    || !southWestCell)
                {
                    return true;
                }
                
                if (westCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::NORTH: {
                if (!northCell
                    || !northEastCell
                    || !northWestCell)
                {
                    return true;
                }
                
                if (northCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::SOUTH: {
                if (!southCell
                    || !southEastCell
                    || !southWestCell)
                {
                    return true;
                }
                
                if (southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                
            case BackTrackEnum::EAST: {
                if (!eastCell
                    || !northEastCell
                    || !southEastCell)
                {
                    return true;
                }
                
                if (eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            default:
                break;
        }
        
        cells.push(northEastCell);
        cells.push(northWestCell);
        cells.push(southEastCell);
        cells.push(southWestCell);
        cells.push(eastCell);
        cells.push(westCell);
        cells.push(northCell);
        cells.push(southCell);
        
        currentCell = cells.front();
        cells.pop();
    }
    
    return false;
}

bool RoutePlotter::areNavCellsOccupiedFromBottomRightCorner(NavigationCell* centerCell, Animal* navigatorAnimal, float radius, BackTrackEnum direction) {
    NavigationCell* centerEastCell = centerCell->getEastCell();
    NavigationCell* centerSouthCell = centerCell->getSouthCell();
    NavigationCell* centerSouthEastCell = centerCell->getSouthEastCell();
    
    if (!centerEastCell
        || !centerSouthCell
        || !centerSouthEastCell)
    {
        return true;
    }
    
    if (centerEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
        || centerSouthCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
        || centerSouthEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
    {
        return true;
    }
    
    if (centerCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal) == true) {
        return true;
    }
    
    std::queue<NavigationCell*> cells;
    //    std::map<NavigationCell*,NavigationCell*> visitedCells; // TODO: make this more efficient by making cells visited

    cells.push(centerEastCell);
    cells.push(centerSouthCell);
    cells.push(centerSouthEastCell);
    
    float halfCollisionCellSize = District::METERES_PER_TILE / NAVIGATION_CELLS_PER_TILE_WIDE;
    
    b2Vec2 halfCellVec(halfCollisionCellSize,halfCollisionCellSize);
    
    for (NavigationCell* currentCell=centerCell;
         b2Distance(centerCell->getBottomRightPoint(), currentCell->getBottomRightPoint()) + 0.001 < radius;)
    {
        NavigationCell* eastCell = currentCell->getEastCell();
        NavigationCell* westCell = currentCell->getWestCell();
        NavigationCell* northCell = currentCell->getNorthCell();
        NavigationCell* southCell = currentCell->getSouthCell();
        
        NavigationCell* northEastCell = currentCell->getNorthEastCell();
//        NavigationCell* northWestCell = currentCell->getNorthWestCell();
        NavigationCell* southEastCell = currentCell->getSouthEastCell();
        NavigationCell* southWestCell = currentCell->getSouthWestCell();
        
        switch (direction) {
            case BackTrackEnum::EAST: {
                if (!eastCell
                    || !southCell
                    || !southEastCell)
                {
                    return true;
                }
                
                if (eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::SOUTH: { //check south and south east
                if (!southCell
                    || !eastCell
                    || !southEastCell)
                {
                    return true;
                }
                
                if (southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
                
            case BackTrackEnum::WEST: { //check west and south west
                if (!westCell
                    || !southCell
                    || !southWestCell)
                {
                    return true;
                }
                
                if (westCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || southWestCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            case BackTrackEnum::NORTH: { //check north and north east
                if (!northCell
                    || !eastCell
                    || !northEastCell)
                {
                    return true;
                }
                
                if (northCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || eastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal)
                    || northEastCell->isOccupiedByAnythingOtherThanEntity(navigatorAnimal))
                {
                    return true;
                }
            }
                break;
                
            default:
                assert(false && "unhandled direction");
                break;
        }
        
        cells.push(eastCell);
        cells.push(westCell);
        cells.push(northCell);
        cells.push(southCell);
        
        currentCell = cells.front();
        cells.pop();
    }
    
    return false;
}