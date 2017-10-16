#ifndef DISTRICT_HPP_
#define DISTRICT_HPP_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include "extlibs/rapidjson/document.h"
#include "SFML/Graphics.hpp"

#include "common.hpp"
#include "PlayEntity.hpp"
#include "Wall.hpp"
#include "ClassChecker.hpp"
#include "PooledPlayEntity.hpp"
#include "Animal.hpp"

class NavigationCell;
class PooledPlayEntityDescriptor;

#define NUM_TILES_WIDE 16
#define NUM_TILES_TALL NUM_TILES_WIDE
#define DISCTRICT_PIXEL_WIDTH (NUM_TILES_WIDE*TILE_PIXEL_DIMENSIONS)
#define DISCTRICT_PIXEL_HEIGHT DISCTRICT_PIXEL_WIDTH
#define VERTICES_PER_QUAD 4
#define TILE_PIXEL_DIMENSIONS 100
#define TILE_COUNT NUM_TILES_WIDE*NUM_TILES_TALL

#define NAVIGATION_CELLS_PER_TILE_WIDE 4
#define NAVIGATION_CELLS_PER_TILE_TALL NAVIGATION_CELLS_PER_TILE_WIDE
#define NAVIGATION_CELLS_PER_DISTRICT_WIDE (NAVIGATION_CELLS_PER_TILE_TALL*NUM_TILES_WIDE)
#define NAVIGATION_CELLS_PER_DISTRICT_TALL NAVIGATION_CELLS_PER_DISTRICT_WIDE
#define NAVIGATION_CELLS_PIXEL_DIMENSIONS (TILE_PIXEL_DIMENSIONS/NAVIGATION_CELLS_PER_TILE_WIDE)


#define FLOORS_PER_DISTRICT 15

// define if you would like to see the navigation mesh drawn to the screen
//#define DISTRICT_NAVIGATION_VISUAL_DEBUG

class District : public PlayEntity {
public:
	enum class Directions {
		NONE, NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST
	};

protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

private:
	static int indexTable[NAVIGATION_CELLS_PER_DISTRICT_WIDE+2][NAVIGATION_CELLS_PER_DISTRICT_TALL+2]; //used for collision cells
	static void populateIndexTable();
	static int indexFromIndexTable(int index, District::Directions direction);

	static sf::Vector2f tilePixelVertexPositions[NAVIGATION_CELLS_PER_DISTRICT_WIDE+1][NAVIGATION_CELLS_PER_DISTRICT_TALL+1];
	static void populateTilePixelVertexPositions();

	sf::FloatRect getTilePixelBounds(int x, int y);

	ACCESSOR(b2Vec2, IndexLocation);
	sf::VertexArray mRoadVertexArray;
	sf::VertexArray mInteriorVertexArray;
	sf::Texture mRoadTexture;
	sf::Texture mInteriorTexture;
	std::vector<Wall*> mWallsToDraw;

	sf::Vector2f mOffset; //offset in screen coordinates

	sf::Color debugDrawColour;

	int** roadTiles;
	int** interiorTiles;
	Wall*** walls;
    std::map<int,std::vector<PooledPlayEntityDescriptor*> > floorToEntityDescriptorsMap;

	NavigationCell ** collisionCells;
	sf::Vector2f** collisionCellWorldCoords;

	void setupCollisionCells();

    void populateWallsAtFloorFromValue(int floor, const rapidjson::Value& outdoorTilesValue);
    void populateDistrictWithEntitesFromJsonValue(const rapidjson::Value& pooledPlayEntitiesValue);

    void unlinkNeighbouringNavMeshes();
    void removeResidentPooledPlayEntities();
    
public:
    ACCESSOR(b2Body*, StaticBody);
    
	static constexpr float METERES_PER_TILE = 2.0f;
	static constexpr float DISTRICT_WIDTH_IN_METERS = NUM_TILES_WIDE*METERES_PER_TILE;
	static constexpr float TILES_PER_METER = 1.0f/METERES_PER_TILE;

	sf::Vector2f position; 	//the last camera world coordinates provided (position is camera center)
	sf::Vector2f dimensions;//the last camera world dimensions provided  (size is camera centered)

	/* District constructor
	 *
	 * Creates a new district at indexLocation.  indexLocation is it's position in district space,
	 * where the center district is located at (0,0)
	 */
	District(PlayContext* playContext, b2Vec2 indexLocation);
	virtual ~District();
    
	virtual void update(float timeStep);
	virtual void draw(sf::RenderTarget& window);

    virtual void teardown();
    
	void setTile(int floor, sf::Vector2f worldPosition, int newTileNumber, std::string tileCategory);
	void setWall(int floor, sf::Vector2f tilePosition, int wallOrientation);
    void setPooledPlayEntityForDescriptor(PooledPlayEntityDescriptor* descriptor);
    void unsetPooledPlayEntityForPersistenceIdentifer(int persistenceIdentifer);
    
    void setNavigationCellOccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity);
    void setNavigationCellUnoccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity);
    
    std::set<NavigationCell*>* mCellsThatHaveBeenOccupied;
    
	void loadFromFile(std::string filePath);
	void saveToFile(std::string filePath);

	void buildVertexArrayWithTexture(int floor, sf::Vector2i topLeft, sf::Vector2i bottomRight, sf::VertexArray& vertexArray, int** tiles, sf::Texture* texture);
	void buildDrawableRegion(int floor, sf::Vector2i topLeft, sf::Vector2i bottomRight);
	void setVisibleRegion(int floor, sf::Vector2f position, sf::Vector2f dimensions);
	sf::Vector2i worldSpaceToTileIndexSpace(sf::Vector2f position);

	NavigationCell * collisionCellAtIndex(int x, int y);
    std::vector<NavigationCell*> getNavigationCellsList();

	std::vector<NavigationCell *> getCollisionCellsTopEdge();
	std::vector<NavigationCell *> getCollisionCellsBottomEdge();
	std::vector<NavigationCell *> getCollisionCellsLeftEdge();
	std::vector<NavigationCell *> getCollisionCellsRightEdge();
	NavigationCell * getNorthEastCornerCollisionCell();
	NavigationCell * getNorthWestCornerCollisionCell();
	NavigationCell * getSouthEastCornerCollisionCell();
	NavigationCell * getSouthWestCornerCollisionCell();

	void linkNorth(District* northDistrict);
	void linkSouth(District* southDistrict);
	void linkEast(District* eastDistrict);
	void linkWest(District* westDistrict);
	void linkNorthEast(District* northEastDistrict);
	void linkNorthWest(District* northWestDistrict);
	void linkSouthEast(District* southEastDistrict);
	void linkSouthWest(District* southWestDistrict);

	//TODO: move to common
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}

	//TODO: move to common
	std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}
    
    class PooledPlayEntityFinderQueryCallback : public b2QueryCallback {
    public:
        GETTER(std::vector<PooledPlayEntity*>, PooledPlayEntitiesFound);
        
        bool ReportFixture(b2Fixture* fixture) {
            if(fixture->GetBody()->GetUserData() == NULL){
                return true; // no user data; keep looking
            }
            
            PlayEntity *playEntity = (PlayEntity*)fixture->GetBody()->GetUserData();
            if(ClassChecker().isKindOfClass<PooledPlayEntity*>(playEntity)){
                mPooledPlayEntitiesFound.push_back(dynamic_cast<PooledPlayEntity*>(playEntity));
            }
            
            return true;
        }
    };
};

#endif /* DISTRICT_HPP_ */
