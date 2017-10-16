#include "DistrictManager.hpp"

#include <fstream>
#include <dirent.h>
#include <vector>

#include <Box2D/Box2d.h>

#include "GameContext.hpp"
#include "Game.hpp"
#include "District.hpp"
#include "OS.hpp"
#include "PlayContext.hpp"
#include "Camera.hpp"
#include "Wall.hpp"
#include "RoutePlotter.hpp"
#include "PooledPlayEntityDescriptor.hpp"

#include "NavigationMeshHelper.hpp"

DistrictManager::DistrictManager(PlayContext* playContext) : mPlayContext(playContext) {
	mCurrentFloor = 0;
	mPosition   = sf::Vector2f(FLT_MIN,FLT_MIN);
	mDimensions = sf::Vector2f(FLT_MIN,FLT_MIN);

	mShouldShowHighlightedTile = false;
	mShouldShowBoxHighlight = false;
	mBoxHighlight.setFillColor(sf::Color::Transparent);
	mBoxHighlight.setOutlineColor(sf::Color(sf::Color::Magenta.r,sf::Color::Magenta.g,sf::Color::Magenta.b, 64));
	mBoxHighlight.setOutlineThickness(3.0f);

	this->mShouldShowBoxHighlight = false;
	mHighlightedTile.setFillColor(sf::Color(255,187,241,128));
	mHighlightedTile.setSize(sf::Vector2f(TILE_PIXEL_DIMENSIONS,TILE_PIXEL_DIMENSIONS));
}

DistrictManager::~DistrictManager() {
	for(auto iter=this->map.begin(); iter!=this->map.end(); iter++){
		this->removeDistrict(iter->second);
	}
}

void DistrictManager::update(float timeStep) {
	for(auto iter=this->map.begin(); iter!=this->map.end(); iter++){
        NavigationMeshHelper(*this->getPlayContext()->getGameContext()->getGame()->getWindow()).clearDistrictNavigationCellsOccupation(iter->second);
        NavigationMeshHelper(*this->getPlayContext()->getGameContext()->getGame()->getWindow()).setAppropriateCellsAsOccupied(this->mPlayContext->getWorld(), this);

        (iter->second)->update(timeStep);
	}
}

/* setVisibleRegion
 *
 * Informs the district manager about the area that is in view.
 */
void DistrictManager::setVisibleRegion(int floor, sf::Vector2f position, sf::Vector2f dimensions){
	//no change to visible region, so ignore
	if(mPosition.x == position.x && mPosition.y == position.y &&
		mDimensions.x == dimensions.x && mDimensions.y == dimensions.y &&
		mCurrentFloor == floor)
    {
		return;
	}
    
	mCurrentFloor = floor;
	mPosition = position;
	mDimensions = dimensions;

    //remove unwanted districts
    this->removeDistrictsNotWithinVisibleRegion();

    //load every district in view
	sf::Vector2i topLeftDistrictIndex = DistrictManager::worldToDistrictIndex(sf::Vector2f(mPosition.x-mDimensions.x/2.0f, mPosition.y-mDimensions.y/2.0f));
	sf::Vector2i bottomRightDistrictIndex = DistrictManager::worldToDistrictIndex(sf::Vector2f(mPosition.x+mDimensions.x/2.0f, mPosition.y+mDimensions.y/2.0f));

	int leftx = topLeftDistrictIndex.x;
	int rightx = bottomRightDistrictIndex.x;
	int topy = topLeftDistrictIndex.y;
	int bottomy = bottomRightDistrictIndex.y;
	for(;leftx<=rightx; leftx++){
		for(topy = topLeftDistrictIndex.y; topy<=bottomy; topy++) {
			this->loadDistrictIntoMap(b2Vec2(leftx,topy));
		}
	}

	//guarantee the loading of the middle tile, and it's surrounding districts
	sf::Vector2i centerIndex = DistrictManager::worldToDistrictIndex( mPosition );
	sf::Vector2i centerUpLeftIndex(centerIndex.x-1, centerIndex.y-1);
	sf::Vector2i centerBottomRightIndex(centerIndex.x+1, centerIndex.y+1);
	leftx = centerUpLeftIndex.x;
	rightx = centerBottomRightIndex.x;
	topy = centerUpLeftIndex.y;
	bottomy = centerBottomRightIndex.y;
	for(;leftx<=rightx;leftx++){
		for(topy = centerUpLeftIndex.y;topy<=bottomy;topy++){
			this->loadDistrictIntoMap(b2Vec2(leftx,topy));
		}
	}

	//pass on the visible region
	for(auto iterator = map.begin(); iterator != map.end(); iterator++) {
		iterator->second->setVisibleRegion(floor, mPosition, mDimensions);
	}
}

void DistrictManager::resetHighlightTileIndicatorSize() {
	mHighlightedTile.setSize(sf::Vector2f(TILE_PIXEL_DIMENSIONS, TILE_PIXEL_DIMENSIONS));
}

/* worldToDistrictIndex
 *
 * Converts between a world position vector to a district index
 *
 */
sf::Vector2i DistrictManager::worldToDistrictIndex(sf::Vector2f position){
	//translate the position because we're working in a translated space
	position.x+=District::DISTRICT_WIDTH_IN_METERS/2.0f;
	position.y+=District::DISTRICT_WIDTH_IN_METERS/2.0f;
	return sf::Vector2i(std::floor(position.x/District::DISTRICT_WIDTH_IN_METERS),
                        std::floor(position.y/District::DISTRICT_WIDTH_IN_METERS));
}

/* draw
 *
 * Draws the contained districts to the RenderTarget
 */
void DistrictManager::draw(sf::RenderTarget& window) {
	for(auto iterator = map.begin(); iterator != map.end(); iterator++) {
		iterator->second->draw(window);
	}

	if(mShouldShowHighlightedTile){
		window.draw(mHighlightedTile);
	}
	if(mShouldShowBoxHighlight){
		window.draw(mBoxHighlight);
	}
}

/** loadDistrictIntoMap
 *
 * Creates and loads an image into the district map, given the districtIndex for the new district to be created.
 * The new district is generated in the correct location, specified by the districtIndex.
 */
void DistrictManager::loadDistrictIntoMap(b2Vec2 districtIndex){
	std::string districtIndexString = DistrictManager::districtIndexToDistrictName(districtIndex);

	//the district already exists in the map, so return
	if(map.count(MapKey(districtIndex.x,districtIndex.y))){
		return;
	}

	std::string filePath = OS::getPwd() + "resources/districts/" + districtIndexString + ".json";

	std::ifstream in(filePath.c_str(), std::ios::in | std::ios::binary);
	District* district;
	if(!in.good()){
		std::cout << "failed to load district file " << districtIndexString  << " loading generic district, instead."<< std::endl;
		district = new District(mPlayContext, districtIndex);
		this->map.insert( std::pair<MapKey, District*>( MapKey(districtIndex.x,districtIndex.y), district) );
	} else {
		std::string districtString((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		std::cout << "loading district file " << filePath << std::endl;
		district = new District(mPlayContext, districtIndex);
		district->loadFromFile(filePath);
		this->map.insert( std::pair<MapKey, District*>( MapKey(districtIndex.x,districtIndex.y), district) );
	}
	this->linkNeighbourDistrictsInMap(district);
}

void DistrictManager::linkNeighbourDistrictsInMap(District* district){
	b2Vec2 northDistrictIndex(district->getIndexLocation().x, district->getIndexLocation().y-1);
	b2Vec2 southDistrictIndex(district->getIndexLocation().x, district->getIndexLocation().y+1);
	b2Vec2 eastDistrictIndex(district->getIndexLocation().x+1, district->getIndexLocation().y);
	b2Vec2 westDistrictIndex(district->getIndexLocation().x-1, district->getIndexLocation().y);
	b2Vec2 northEastDistrictIndex(district->getIndexLocation().x+1, district->getIndexLocation().y-1);
	b2Vec2 northWestDistrictIndex(district->getIndexLocation().x-1, district->getIndexLocation().y-1);
	b2Vec2 southEastDistrictIndex(district->getIndexLocation().x+1, district->getIndexLocation().y+1);
	b2Vec2 southWestDistrictIndex(district->getIndexLocation().x-1, district->getIndexLocation().y+1);

    if(this->map.count(MapKey(northDistrictIndex.x,northDistrictIndex.y))) {
        district->linkNorth(this->map[MapKey(northDistrictIndex.x,northDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(southDistrictIndex.x,southDistrictIndex.y))) {
        district->linkSouth(this->map[MapKey(southDistrictIndex.x,southDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(eastDistrictIndex.x,eastDistrictIndex.y))) {
        district->linkEast(this->map[MapKey(eastDistrictIndex.x,eastDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(westDistrictIndex.x,westDistrictIndex.y))) {
        district->linkWest(this->map[MapKey(westDistrictIndex.x,westDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(northEastDistrictIndex.x,northEastDistrictIndex.y))) {
        district->linkNorthEast(this->map[MapKey(northEastDistrictIndex.x,northEastDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(northWestDistrictIndex.x,northWestDistrictIndex.y))) {
        district->linkNorthWest(this->map[MapKey(northWestDistrictIndex.x,northWestDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(southEastDistrictIndex.x,southEastDistrictIndex.y))) {
        district->linkSouthEast(this->map[MapKey(southEastDistrictIndex.x,southEastDistrictIndex.y)]);
    }
    if(this->map.count(MapKey(southWestDistrictIndex.x,southWestDistrictIndex.y))) {
        district->linkSouthWest(this->map[MapKey(southWestDistrictIndex.x,southWestDistrictIndex.y)]);
    }
}

/* setTile
 *
 * Sets a tile in the specified world position, to the new tile number
 */
void DistrictManager::setTile(int floor, sf::Vector2f worldPosition, int newTileNumber, std::string tileCategory){
	sf::Vector2i sfDistrictIndex = DistrictManager::worldToDistrictIndex( worldPosition );
    
    b2Vec2 districtIndex(sfDistrictIndex.x,sfDistrictIndex.y);
    
	if(map.find(MapKey(districtIndex.x,districtIndex.y)) == map.end()) {
		return; //item not found in the map
	}
	District* district = map.find(MapKey(districtIndex.x,districtIndex.y))->second;

	//reorient the position so that it's relative to the district
	sf::Vector2f tilePosition(worldPosition.x - districtIndex.x*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f,
							  worldPosition.y - districtIndex.y*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f);
	district->setTile(floor, tilePosition, newTileNumber, tileCategory);
}

void DistrictManager::setWall(int floor, sf::Vector2f worldPosition, int newWallOrientationNumber) {
	sf::Vector2i sfDistrictIndex = DistrictManager::worldToDistrictIndex( worldPosition );
    
    b2Vec2 districtIndex(sfDistrictIndex.x,sfDistrictIndex.y);
    
	if(map.find(MapKey(districtIndex.x,districtIndex.y)) == map.end()) {
		return; //item not found in the map
	}
	District* district = map.find(MapKey(districtIndex.x,districtIndex.y))->second;

	//reorient the position so that it's relative to the district
	sf::Vector2f tilePosition(worldPosition.x - districtIndex.x*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f,
							  worldPosition.y - districtIndex.y*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f);
	district->setWall(floor, tilePosition, newWallOrientationNumber);
}

void DistrictManager::setPooledPlayEntityForDescriptor(PooledPlayEntityDescriptor* descriptor) {
    sf::Vector2f worldPosition(descriptor->getX(), descriptor->getY());
    
    sf::Vector2i sfDistrictIndex = DistrictManager::worldToDistrictIndex( worldPosition );
    
    b2Vec2 districtIndex(sfDistrictIndex.x,sfDistrictIndex.y);
    
    if(map.find(MapKey(districtIndex.x,districtIndex.y)) == map.end()) {
        return; //item not found in the map
    }
    District* district = map.find(MapKey(districtIndex.x,districtIndex.y))->second;
    
    district->setPooledPlayEntityForDescriptor(descriptor);
    
}

void DistrictManager::unsetPooledPlayEntityForPersistenceIdentifer(int persistenceIdentifer) {
    for (auto pair : map ) {
        District* district = pair.second;
        district->unsetPooledPlayEntityForPersistenceIdentifer(persistenceIdentifer);
    }
}

void DistrictManager::setNavigationCellOccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity) {
    District* district = this->districtForLocation(location);
    
    if (district == NULL) {
        return;
    }
    
    district->setNavigationCellOccupiedAtLocationWithEntity(location, playEntity);
}

void DistrictManager::setNavigationCellUnoccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity) {
    District* district = this->districtForLocation(location);
    
    if (district == NULL) {
        return;
    }
    
    district->setNavigationCellUnoccupiedAtLocationWithEntity(location, playEntity);
}

District* DistrictManager::districtForLocation(b2Vec2 location) {
    sf::Vector2i sfDistrictIndex = DistrictManager::worldToDistrictIndex( sf::Vector2f(location.x,location.y) );
    
    b2Vec2 districtIndex(sfDistrictIndex.x,sfDistrictIndex.y);
    
//    if(map.find(MapKey(districtIndex.x,districtIndex.y)) == map.end()) {
//        return NULL; //item not found in the map
//    }
    
    auto pair = map.find(MapKey(districtIndex.x,districtIndex.y));
    
    if (pair == map.end()) {
        return NULL;
    }
    
    District* district = pair->second;
    
    return district;
}

/* highlightTileAtWorldPoint
 *
 * places a purple indicator over the tile at worldPosition
 */
void DistrictManager::highlightTileAtWorldPoint(sf::Vector2f worldPosition){
	sf::Vector2i sfDistrictIndex = DistrictManager::worldToDistrictIndex( worldPosition );
    
    b2Vec2 districtIndex(sfDistrictIndex.x,sfDistrictIndex.y);
	if(map.find(MapKey(districtIndex.x,districtIndex.y)) == map.end()) {
		return; //item not found in the map
	}

	sf::Vector2f tileLockedWorldPosition(worldPosition);
	tileLockedWorldPosition.x = std::floor(tileLockedWorldPosition.x/2.0f);
	tileLockedWorldPosition.y = std::floor(tileLockedWorldPosition.y/2.0f);
	tileLockedWorldPosition.x = tileLockedWorldPosition.x*2;
	tileLockedWorldPosition.y = tileLockedWorldPosition.y*2;
	sf::Vector2f screenCoords = mPlayContext->getCamera()->worldToScreen(tileLockedWorldPosition);
	mHighlightedTile.setPosition(screenCoords);
}

/* highlightTileAtWorldPoint
 *
 * places a purple indicator over the tile at worldPosition
 */
void DistrictManager::highlightTilesInRectangle(sf::FloatRect rect){
	sf::Vector2f worldPosition(rect.left, rect.top);

	sf::Vector2f boxScreenPosition = mPlayContext->getCamera()->worldToScreen(sf::Vector2f(rect.left, rect.top));
	sf::Vector2f boxScreenSize = mPlayContext->getCamera()->worldToScreen(sf::Vector2f(rect.width, rect.height));
	mBoxHighlight.setPosition(boxScreenPosition);
	mBoxHighlight.setSize(boxScreenSize);

	sf::Vector2f startWorldPosition(rect.left,rect.top);
	sf::Vector2f endWorldPosition(rect.left+rect.width,rect.top+rect.height);
	float startX = minX(startWorldPosition,endWorldPosition);
	float startY = minY(startWorldPosition,endWorldPosition);
	float endX = maxX(startWorldPosition,endWorldPosition);
	float endY = maxY(startWorldPosition,endWorldPosition);

	sf::Vector2f tileLockedTopLeftWorldPosition(std::floor(startX/2.0f)*2.0f, std::floor(startY/2.0f)*2.0f);
	sf::Vector2f tileLockedBottomRightWorldPosition(std::ceil(endX/2.0f)*2.0f, std::ceil(endY/2.0f)*2.0f);

	sf::Vector2f screenCoords = mPlayContext->getCamera()->worldToScreen(tileLockedTopLeftWorldPosition);
	sf::Vector2f screenSize = mPlayContext->getCamera()->worldToScreen( sf::Vector2f(tileLockedBottomRightWorldPosition.x-tileLockedTopLeftWorldPosition.x,
																				 tileLockedBottomRightWorldPosition.y-tileLockedTopLeftWorldPosition.y) );
	mHighlightedTile.setPosition(screenCoords);
	mHighlightedTile.setSize(screenSize);
}

/* districtIndexToDistrictName
 *
 * Creates the appropriate district name, given the districtIndex
 */
std::string DistrictManager::districtIndexToDistrictName(b2Vec2 districtIndex){
	std::stringstream stringStream;
	stringStream << "District_" << districtIndex.x << "_" << districtIndex.y;
	std::string districtString = stringStream.str();
	return districtString;
}

/* removeDistrictsNotWithinVisibleRegion
 *
 * Removes all of the districts that are not the camera center district, not in the view, and that are not immediately perpendicular to the
 * districts surrounding the camera centered district.
 */
void DistrictManager::removeDistrictsNotWithinVisibleRegion(){
	//find the indexes on screen
	sf::Vector2i positionIndex = DistrictManager::worldToDistrictIndex( mPosition );
	sf::Vector2i sfTopLeftDistrictIndex = DistrictManager::worldToDistrictIndex(sf::Vector2f(mPosition.x-mDimensions.x/2.0f, mPosition.y-mDimensions.y/2.0f));
	sf::Vector2i sfBottomRightDistrictIndex = DistrictManager::worldToDistrictIndex(sf::Vector2f(mPosition.x+mDimensions.x/2.0f, mPosition.y+mDimensions.y/2.0f));
    
    b2Vec2 topLeftDistrictIndex(sfTopLeftDistrictIndex.x,sfTopLeftDistrictIndex.y);
    b2Vec2 bottomRightDistrictIndex(sfBottomRightDistrictIndex.x,sfBottomRightDistrictIndex.y);
    
	//include the surrounding districts, from the centered district
	if(topLeftDistrictIndex.x == positionIndex.x){
		topLeftDistrictIndex.x = topLeftDistrictIndex.x-1;
	}
	if(topLeftDistrictIndex.y == positionIndex.y){
		topLeftDistrictIndex.y = topLeftDistrictIndex.y-1;
	}
	if(bottomRightDistrictIndex.x == positionIndex.x){
		bottomRightDistrictIndex.x = bottomRightDistrictIndex.x+1;
	}
	if(bottomRightDistrictIndex.y == positionIndex.y){
		bottomRightDistrictIndex.y = bottomRightDistrictIndex.y+1;
	}

	for(auto iterator = map.begin(); iterator != map.end(); iterator++) {
		District* district = iterator->second;
		b2Vec2 index = district->getIndexLocation();
		if(index.x<topLeftDistrictIndex.x || index.x>bottomRightDistrictIndex.x ||
				index.y<topLeftDistrictIndex.y || index.y>bottomRightDistrictIndex.y){
			removeDistrict(district);
			map.erase(iterator);
			iterator=map.begin();
		}
	}
}

/* removeDistrict
 *
 * Performs a clean removal of the district, by completing additional cleanup, such as unlinking the navigation mesh from it's neighbouring districts
 */
void DistrictManager::removeDistrict(District* district){
	b2Vec2 index = district->getIndexLocation();
	std::string districtString = DistrictManager::districtIndexToDistrictName(index) + ".json";
	std::string filePath = OS::getPwd() + "resources/districts/" + districtString;

	district->saveToFile(filePath);

    district->teardown();
    
    delete district;
}

// TODO: you don't need to pass the diameter of an animal and also the animal; only pass the animal.
std::vector<NavigationCell *> DistrictManager::plotRouteFromStartToEnd(b2Vec2 start, b2Vec2 end, Animal* navigatorAnimal, float navigatorDiameter)
{
    CollisionCellFinderQueryCallback callbackStart(start);
    b2AABB startAABB;
    startAABB.upperBound = start;
    startAABB.lowerBound = start + b2Vec2(0.1,0.1);
    mPlayContext->getWorld()->QueryAABB(&callbackStart, startAABB);
    NavigationCell *startCell = callbackStart.getCellFound();
    
    if (!startCell) {
        return std::vector<NavigationCell *>();
    }
    
    assert(startCell != NULL);
    
    CollisionCellFinderQueryCallback callbackEnd(end);
    b2AABB endAABB;
    endAABB.upperBound = end;
    endAABB.lowerBound = end + b2Vec2(0.1,0.1);
    mPlayContext->getWorld()->QueryAABB(&callbackEnd, endAABB);
    NavigationCell *endCell = callbackEnd.getCellFound();
    assert(endCell != NULL);
    
    RoutePlotter routePlotter;
    return routePlotter.plotRouteBetweenNavCells(startCell, endCell, navigatorAnimal, navigatorDiameter);
}
