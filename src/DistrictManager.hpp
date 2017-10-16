#ifndef DISTRICTMANAGER_HPP_
#define DISTRICTMANAGER_HPP_

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "NavigationCell.hpp"
#include "ClassChecker.hpp"

class Animal;
class PlayContext;
class District;
class PooledPlayEntityDescriptor;

typedef std::pair<int, int> MapKey;

class DistrictManager {
protected:
	ACCESSOR(PlayContext*, PlayContext);
	ACCESSOR(bool, ShouldShowBoxHighlight);
	ACCESSOR(bool, ShouldShowHighlightedTile);

private:
	int mCurrentFloor;
	sf::Vector2f mPosition;
	sf::Vector2f mDimensions;

	void loadDistrictIntoMap(b2Vec2 index);
	void linkNeighbourDistrictsInMap(District* district);
	void removeDistrictsNotWithinVisibleRegion();
	void removeDistrict(District* district);

	static std::string districtIndexToDistrictName(b2Vec2 districtIndex);

	sf::RectangleShape mHighlightedTile;
	sf::RectangleShape mBoxHighlight;
    
    District* districtForLocation(b2Vec2 location);

public:
    std::map<MapKey, District*> map;
    
	DistrictManager(PlayContext* playContext);
	virtual ~DistrictManager();

	virtual void update(float timestep);

	void setVisibleRegion(int floor, sf::Vector2f position, sf::Vector2f dimensions);

	static sf::Vector2i worldToDistrictIndex(sf::Vector2f position);

	void setTile(int floor, sf::Vector2f worldPosition, int newTileNumber, std::string tileCategory);
	void setWall(int floor, sf::Vector2f worldPosition, int newWallOrientationNumber);

    void setPooledPlayEntityForDescriptor(PooledPlayEntityDescriptor* descriptor);
    void unsetPooledPlayEntityForPersistenceIdentifer(int persistenceIdentifer);
    
    void setNavigationCellOccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity);
    void setNavigationCellUnoccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity);
    
	void highlightTileAtWorldPoint(sf::Vector2f worldPosition);
	void highlightTilesInRectangle(sf::FloatRect rect);
	void resetHighlightTileIndicatorSize();

	void draw(sf::RenderTarget& window);
    
    /*** returns a route described by navigation cells
     */
    std::vector<NavigationCell *> plotRouteFromStartToEnd(b2Vec2 start, b2Vec2 end, Animal* navigatorAnimal, float navigatorDiameter);
    
private:
    class CollisionCellFinderQueryCallback : public b2QueryCallback {
    private:
        b2Vec2 mPointOfInterest;
        
    public:
        GETTER_AND_DEFAULT_NULL(NavigationCell *, CellFound);
        
        CollisionCellFinderQueryCallback(b2Vec2 pointOfInterest) {
            this->mPointOfInterest = pointOfInterest;
        }
        
        bool ReportFixture(b2Fixture* fixture) {
            if(fixture->GetBody()->GetUserData() == NULL){
                return true; // no user data; keep looking
            }
            
            PlayEntity *playEntity = (PlayEntity*)fixture->GetBody()->GetUserData();
            if(ClassChecker().isKindOfClass<NavigationCell *>(playEntity) == false){
                return true; // not a nav cell; keep looking
            }
            
            NavigationCell *cell = dynamic_cast<NavigationCell *>(playEntity);
            
            if (cell->getBodyFixture()->TestPoint(this->mPointOfInterest)) {
                mCellFound = cell;
                return false; // found our collision cell; stop looking
            }
            
            return true;
        }
    };
};

#endif /* DISTRICTMANAGER_HPP_ */
