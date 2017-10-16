

#ifndef __gtmiami__RoutePlotter__
#define __gtmiami__RoutePlotter__

#include <stdio.h>
#include <vector>
#include <map>
#include <queue>

#include "common.hpp"
#include "NavigationCell.hpp"

#include "District.hpp"

#define MAX_PATH_LENGTH NUM_TILES_WIDE*NUM_TILES_WIDE*NAVIGATION_CELLS_PER_TILE_WIDE*NAVIGATION_CELLS_PER_TILE_WIDE

class RoutePlotter {
    
    enum BackTrackEnum {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        NORTH_EAST,
        NORTH_WEST,
        SOUTH_EAST,
        SOUTH_WEST,
        
        NONE
    };
    
public:
    std::vector<NavigationCell *> plotRouteBetweenNavCells(NavigationCell * start, NavigationCell * trueEnd, Animal* navigatorAnimal, float navigatorDiameter);
    
private:
    std::map<NavigationCell *,BackTrackEnum> mVisitedNavCells;
    std::queue<NavigationCell *> mQueue;
    
    void markNavCellAsVisited(NavigationCell *cell, BackTrackEnum backTrackDirection);
    bool isNavCellVisited(NavigationCell *navCell);
    void clearData();
    
    bool areNavCellsOccupiedFromCenter(NavigationCell* centerCell, Animal* navigatorAnimal, float radius, BackTrackEnum direction);
    bool areNavCellsOccupiedFromBottomRightCorner(NavigationCell* centerCell, Animal* navigatorAnimal, float radius, BackTrackEnum direction);
};

#endif
