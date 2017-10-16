#include "District.hpp"

#include <fstream>

#include "extlibs/rapidjson/filestream.h"
#include "extlibs/rapidjson/document.h"
#include "extlibs/rapidjson/prettywriter.h"
#include "extlibs/rapidjson/stringbuffer.h"

#include "GameContext.hpp"
#include "PlayContext.hpp"
#include "ResourceManager.hpp"
#include "Factory.hpp"
#include "Camera.hpp"
#include "NavigationCell.hpp"
#include "PooledPlayEntityDescriptor.hpp"
#include "EntityManager.hpp" 
#include "PooledPlayEntityDescriptorParser.hpp"
#include "ButtonGUI.hpp"

rapidjson::Value* valueForTileSetAtFloor(rapidjson::Document* jsonDoc, int** outdoorTileSet, int floor)
{
    rapidjson::Value *valueArray = new rapidjson::Value(rapidjson::kArrayType);
    for(int y=0; y<NUM_TILES_WIDE; y++) {
        for(int x=0; x<NUM_TILES_WIDE; x++) {
            int tileValue = outdoorTileSet[floor][x + y*NUM_TILES_TALL];
            valueArray->PushBack(tileValue, jsonDoc->GetAllocator());
        }
    }
    
    return valueArray;
}

rapidjson::Value* valueForWallsAtFloor(rapidjson::Document* jsonDoc, Wall*** walls, int floor)
{
    rapidjson::Value *valueArray = new rapidjson::Value(rapidjson::kArrayType);
    for(int y=0; y<NUM_TILES_WIDE; y++) {
        for(int x=0; x<NUM_TILES_WIDE; x++) {
            Wall* wall = walls[floor][x + y*NUM_TILES_TALL];
            if(wall != NULL){
                valueArray->PushBack(wall->getWallOrientation(), jsonDoc->GetAllocator());
            } else {
                valueArray->PushBack(Wall::WallOrientations::NONE, jsonDoc->GetAllocator());
            }
        }
    }
    
    return valueArray;
}

void populateTilesetAtFloorFromValue(int** tileset, int floor, const rapidjson::Value& outdoorTilesValue) {
    rapidjson::Value::ConstValueIterator itr = outdoorTilesValue.Begin();
    
    for(int j=0; itr!=outdoorTilesValue.End(); j++){
        int x = j % NUM_TILES_WIDE;
        int y = j / NUM_TILES_TALL;
        int tileID = itr->GetInt();
        itr++;
        tileset[floor][x+y*NUM_TILES_TALL] = tileID;
    }
}

void District::populateWallsAtFloorFromValue(int floor, const rapidjson::Value& outdoorTilesValue) {
    rapidjson::Value::ConstValueIterator itr = outdoorTilesValue.Begin();
    
    for(int j=0; itr!=outdoorTilesValue.End(); j++){
        int x = j % NUM_TILES_WIDE;
        int y = j / NUM_TILES_TALL;
        int tileID = itr->GetInt();
        itr++;
        
        b2Vec2 localPosition(x*METERES_PER_TILE, y*METERES_PER_TILE);
        sf::Vector2f offset = Camera::screenToWorld(mOffset);
        b2Vec2 position(offset.x + localPosition.x, offset.y + localPosition.y);
        
        
        sf::Vector2f tilePosition(position.x - mIndexLocation.x*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f,
                                  
                                  position.y - mIndexLocation.y*District::DISTRICT_WIDTH_IN_METERS + District::DISTRICT_WIDTH_IN_METERS/2.0f);
        
        if(tileID != Wall::WallOrientations::NONE) {
            this->setWall(floor, tilePosition, tileID);
        }
    }
}

void District::populateDistrictWithEntitesFromJsonValue(const rapidjson::Value& pooledPlayEntitiesValue) {
    for (rapidjson::Value::ConstValueIterator itr = pooledPlayEntitiesValue.Begin();
         itr != pooledPlayEntitiesValue.End();
         ++itr)
    {
        PooledPlayEntityDescriptor* pooledPlayEntityDescriptor = PooledPlayEntityDescriptorParser().descriptorForJsonDescriptorValue(itr);
        this->setPooledPlayEntityForDescriptor(pooledPlayEntityDescriptor);
    }
}

int District::indexTable[NAVIGATION_CELLS_PER_DISTRICT_WIDE+2][NAVIGATION_CELLS_PER_DISTRICT_TALL+2] = { { 0 } };

sf::Vector2f District::tilePixelVertexPositions[NAVIGATION_CELLS_PER_DISTRICT_WIDE+1][NAVIGATION_CELLS_PER_DISTRICT_TALL+1] = { { sf::Vector2f() } };

void District::populateIndexTable(){
	int incrementer = 0;
	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL+2; y++){
		for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE+2; x++){
			if(y==0 || x==0 || y==NAVIGATION_CELLS_PER_DISTRICT_TALL+1 || x==NAVIGATION_CELLS_PER_DISTRICT_TALL+1){
				indexTable[y][x] = -1;
			} else {
				indexTable[y][x] = incrementer;
				incrementer+=1;
			}
		}
	}
}

/* populateTileWorldPosition
 *
 *  Fills tileWorldPositions with the non-offset vector position values.
 *  Used to avoid recalculating positions which will create seams in OpenGL
 */
void District::populateTilePixelVertexPositions(){
	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL+1; y++){
		for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE+1; x++){
			tilePixelVertexPositions[y][x] = sf::Vector2f(x*TILE_PIXEL_DIMENSIONS, y*TILE_PIXEL_DIMENSIONS);
		}
	}
}

/** getTileBounds
 *
 * returns a district offsetted bounded rectangle with (top, bottom, left, right) coordinates.
 * FloatRect uses the parameters top, left, width, and height.  In our bounded rectangle,
 * top = top, left = left, bottom = height, right = width.
 */
sf::FloatRect District::getTilePixelBounds(int tileIndexX, int tileIndexY){
	sf::FloatRect rect;
	sf::Vector2f topLeft = District::tilePixelVertexPositions[tileIndexY][tileIndexX];
	sf::Vector2f bottomRight = District::tilePixelVertexPositions[tileIndexY+1][tileIndexX+1];

	rect.left = topLeft.x + mOffset.x;
	rect.top = topLeft.y + mOffset.y;
	rect.width = bottomRight.x + mOffset.x;
	rect.height = bottomRight.y + mOffset.y;
	return rect;
}

/** District constructor
 *
 * Creates a District where it's indexLocation is it's center relative (0,0)
 */
District::District(PlayContext* playContext, b2Vec2 indexLocation) :  mIndexLocation(indexLocation) {
	this->setPlayContext(playContext);

	mOffset = sf::Vector2f(indexLocation.x*DISCTRICT_PIXEL_WIDTH - DISCTRICT_PIXEL_WIDTH/2.0f, indexLocation.y*DISCTRICT_PIXEL_WIDTH - DISCTRICT_PIXEL_HEIGHT/2.0f);

	sf::Vector2f offsetCenter = sf::Vector2f(indexLocation.x*DISCTRICT_PIXEL_WIDTH, indexLocation.y*DISCTRICT_PIXEL_WIDTH);
	sf::Vector2f districtWorldPositionCenter = Camera::screenToWorld(sf::Vector2f(offsetCenter.x, offsetCenter.y));

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(districtWorldPositionCenter.x, districtWorldPositionCenter.y);
	bodyDef.type = b2_dynamicBody;
	mBody = mPlayContext->getWorld()->CreateBody(&bodyDef);
	mBody->SetUserData(this);

    b2BodyDef staticBodyDef;
    staticBodyDef.position.Set(districtWorldPositionCenter.x, districtWorldPositionCenter.y);
    staticBodyDef.type = b2_staticBody;
    mStaticBody = mPlayContext->getWorld()->CreateBody(&staticBodyDef);
    mStaticBody->SetUserData(this);
    
	b2PolygonShape bodySensorShape;
	bodySensorShape.SetAsBox(NUM_TILES_WIDE, NUM_TILES_TALL);
	b2FixtureDef bodySensorFixtureDef;
    bodySensorFixtureDef.friction = 0.0f;
	bodySensorFixtureDef.shape = &bodySensorShape;
	bodySensorFixtureDef.isSensor = true;
	mBody->CreateFixture(&bodySensorFixtureDef);

	//initialize indexTable and world positions if they aren't already set
	if(District::indexTable[0][0]==0){
		District::populateIndexTable();
		District::populateTilePixelVertexPositions();
	}

	//debugDrawColour = sf::Color(std::rand()%255,std::rand()%255,std::rand()%255);
	debugDrawColour = sf::Color::Green;

	position = sf::Vector2f(0,0);
	dimensions = sf::Vector2f(0,0);

	roadTiles = new int*[FLOORS_PER_DISTRICT];
	interiorTiles = new int*[FLOORS_PER_DISTRICT];
	walls = new Wall**[FLOORS_PER_DISTRICT];
	for(int i=0; i<FLOORS_PER_DISTRICT; i++){
		roadTiles[i] = new int[TILE_COUNT];
		std::fill(roadTiles[i], roadTiles[i]+TILE_COUNT, 0);

		interiorTiles[i] = new int[TILE_COUNT];
		std::fill(interiorTiles[i], interiorTiles[i]+TILE_COUNT, 0);

		walls[i] = new Wall*[TILE_COUNT];
		for(int j=0; j<TILE_COUNT; j++){
			walls[i][j] = NULL;
		}
	}
    
    for (int i=0; i<FLOORS_PER_DISTRICT; i++) {
        floorToEntityDescriptorsMap[i] = std::vector<PooledPlayEntityDescriptor*>();
    }

    
    int collisionTileCount = NAVIGATION_CELLS_PER_DISTRICT_WIDE*NAVIGATION_CELLS_PER_DISTRICT_TALL;
	collisionCells = new NavigationCell *[collisionTileCount];
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
			sf::Vector2f districtWorldPosition = Camera::screenToWorld(sf::Vector2f(mOffset.x, mOffset.y));
			sf::Vector2f worldPositionOffset = Camera::screenToWorld(sf::Vector2f(x*((float)NAVIGATION_CELLS_PIXEL_DIMENSIONS)+((float)NAVIGATION_CELLS_PIXEL_DIMENSIONS)/2.0f,
                                                                                  y*((float)NAVIGATION_CELLS_PIXEL_DIMENSIONS)+((float)NAVIGATION_CELLS_PIXEL_DIMENSIONS)/2.0f));

            float widthAndHeight = METERES_PER_TILE/((float)NAVIGATION_CELLS_PER_TILE_WIDE);
            sf::Vector2f worldFinalPosition(districtWorldPosition.x+worldPositionOffset.x,// - widthAndHeight/2.0f,
                                            districtWorldPosition.y+worldPositionOffset.y);// - widthAndHeight/2.0f);
            
            
			collisionCells[x+y*NAVIGATION_CELLS_PER_DISTRICT_WIDE] = new NavigationCell(playContext,
                                                                                      b2Vec2(worldFinalPosition.x,worldFinalPosition.y),
                                                                                      widthAndHeight);
		}
	}
	this->setupCollisionCells();

	mRoadTexture.loadFromImage(*playContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::tilemap_png));
	mRoadTexture.setSmooth(false);
	mRoadVertexArray.setPrimitiveType(sf::Quads);
	mRoadVertexArray.resize(NUM_TILES_WIDE * NUM_TILES_TALL * VERTICES_PER_QUAD);

	mInteriorTexture.loadFromImage(*playContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::interiorTilemap_png));
	mInteriorTexture.setSmooth(false);
	mInteriorVertexArray.setPrimitiveType(sf::Quads);
	mInteriorVertexArray.resize(NUM_TILES_WIDE * NUM_TILES_TALL * VERTICES_PER_QUAD);

    mCellsThatHaveBeenOccupied = new std::set<NavigationCell*>();
}

District::~District() {
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
			delete collisionCells[x+y*NAVIGATION_CELLS_PER_DISTRICT_WIDE];
		}
	}

    // remove the walls before you destory the body, because the walls destroy their particular "wall" fixtures.
    
    for(int i=0; i<FLOORS_PER_DISTRICT; i++){
    // TODO: clean up walls, roadTiles, and interiorTiles
        
        for(int j=0; j<TILE_COUNT; j++){
            Wall* wall = walls[i][j];
            if(wall) {
                delete wall;
                wall = NULL;
            }
        }
    }

    mPlayContext->getWorld()->DestroyBody(mBody);
    mPlayContext->getWorld()->DestroyBody(mStaticBody);
}

void District::update(float timeStep) {
	PlayEntity::update(timeStep);

	for(Wall* wall : mWallsToDraw) {
		wall->update(timeStep);
	}
}

void District::draw(sf::RenderTarget& window){
	PlayEntity::draw(window);

	window.draw(mRoadVertexArray, &mRoadTexture);
	window.draw(mInteriorVertexArray, &mInteriorTexture);

	for(Wall* wall : mWallsToDraw) {
		wall->draw(window);
	}
    
    for (auto iter=mCellsThatHaveBeenOccupied->begin(); iter!=mCellsThatHaveBeenOccupied->end(); iter++) {
        NavigationCell* navCell = *iter;
        navCell->draw(window);
    }
        
#ifdef DISTRICT_NAVIGATION_VISUAL_DEBUG
	sf::CircleShape nodeShape(4);
	sf::RectangleShape edgeNorthShape;
	sf::RectangleShape edgeSouthShape;
	sf::RectangleShape edgeEastShape;
	sf::RectangleShape edgeWestShape;
	sf::RectangleShape edgeNorthEastShape;
	sf::RectangleShape edgeNorthWestShape;
	sf::RectangleShape edgeSouthEastShape;
	sf::RectangleShape edgeSouthWestShape;

	edgeNorthShape.rotate(-180.0f);
	edgeWestShape.rotate(180.0f);
	edgeNorthEastShape.rotate(-45);
	edgeNorthWestShape.rotate(-135);
	edgeSouthEastShape.rotate(45);
	edgeSouthWestShape.rotate(135);

	nodeShape.setFillColor(debugDrawColour);
	edgeNorthShape.setFillColor(debugDrawColour);
	edgeSouthShape.setFillColor(debugDrawColour);
	edgeEastShape.setFillColor(debugDrawColour);
	edgeWestShape.setFillColor(debugDrawColour);
	edgeNorthEastShape.setFillColor(debugDrawColour);
	edgeNorthWestShape.setFillColor(debugDrawColour);
	edgeSouthEastShape.setFillColor(debugDrawColour);
	edgeSouthWestShape.setFillColor(debugDrawColour);

	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
		for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
			int index = x+y*NAVIGATION_CELLS_PER_DISTRICT_WIDE;

			sf::Vector2f screenPosition = Camera::worldToScreen(collisionCells[index]->getPosition());
			sf::Vector2f northScreenPosition;
			sf::Vector2f southScreenPosition;
			sf::Vector2f eastScreenPosition;
			sf::Vector2f westScreenPosition;
			sf::Vector2f northEastScreenPosition;
			sf::Vector2f northWestScreenPosition;
			sf::Vector2f southEastScreenPosition;
			sf::Vector2f southWestScreenPosition;
			if(collisionCells[index]->getNorthCell()){
				northScreenPosition = Camera::worldToScreen(collisionCells[index]->getNorthCell()->getPosition());
			}
			if(collisionCells[index]->getSouthCell()){
				southScreenPosition = Camera::worldToScreen(collisionCells[index]->getSouthCell()->getPosition());
			}
			if(collisionCells[index]->getEastCell()){
				eastScreenPosition = Camera::worldToScreen(collisionCells[index]->getEastCell()->getPosition());
			}
			if(collisionCells[index]->getWestCell()){
				westScreenPosition = Camera::worldToScreen(collisionCells[index]->getWestCell()->getPosition());
			}
			if(collisionCells[index]->getNorthEastCell()){
				northEastScreenPosition = Camera::worldToScreen(collisionCells[index]->getNorthEastCell()->getPosition());
			}
			if(collisionCells[index]->getNorthWestCell()){
				northWestScreenPosition = Camera::worldToScreen(collisionCells[index]->getNorthWestCell()->getPosition());
			}
			if(collisionCells[index]->getSouthEastCell()){
				southEastScreenPosition = Camera::worldToScreen(collisionCells[index]->getSouthEastCell()->getPosition());
			}
			if(collisionCells[index]->getSouthWestCell()){
				southWestScreenPosition = Camera::worldToScreen(collisionCells[index]->getSouthWestCell()->getPosition());
			}

			nodeShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeNorthShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeSouthShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeEastShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeWestShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeNorthEastShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeNorthWestShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeSouthEastShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));
			edgeSouthWestShape.setPosition(sf::Vector2f(screenPosition.x,screenPosition.y));

			sf::Vector2f nodeWorldPosition = collisionCells[index]->getPosition();
			sf::Vector2f topLeftPosition(this->position.x-this->dimensions.x/2.0f,this->position.y-this->dimensions.y/2.0f);
			if(sf::FloatRect(topLeftPosition, this->dimensions).contains(nodeWorldPosition)){
				if(collisionCells[index]->getNorthCell()){
					sf::Vector2f size = sf::Vector2f(std::max(std::abs(northScreenPosition.x-nodeShape.getPosition().x),1.0f), std::max(std::abs(northScreenPosition.y-nodeShape.getPosition().y),1.0f));
					edgeNorthShape.setSize(size);
					edgeNorthShape.setFillColor(sf::Color::Yellow);
					window.draw(edgeNorthShape,&mRoadTexture);
				}
				if(collisionCells[index]->getSouthCell()){
					sf::Vector2f size = sf::Vector2f(std::max(std::abs(southScreenPosition.x-nodeShape.getPosition().x),1.0f), std::max(std::abs(southScreenPosition.y-nodeShape.getPosition().y),1.0f));
					edgeSouthShape.setSize(size);
					edgeSouthShape.setFillColor(sf::Color::Green);
					window.draw(edgeSouthShape,&mRoadTexture);
				}
				if(collisionCells[index]->getEastCell()){
					sf::Vector2f size = sf::Vector2f(std::max(std::abs(eastScreenPosition.x-nodeShape.getPosition().x),1.0f), std::max(std::abs(eastScreenPosition.y-nodeShape.getPosition().y),1.0f));
					edgeEastShape.setSize(size);
					edgeEastShape.setFillColor(sf::Color::Red);
					window.draw(edgeEastShape,&mRoadTexture);
				}
				if(collisionCells[index]->getWestCell()){
					sf::Vector2f size = sf::Vector2f(std::max(std::abs(westScreenPosition.x-nodeShape.getPosition().x),1.0f), std::max(std::abs(westScreenPosition.y-nodeShape.getPosition().y),1.0f));
					edgeWestShape.setSize(size);
					edgeWestShape.setFillColor(sf::Color::Cyan);
					window.draw(edgeWestShape,&mRoadTexture);
				}
				if(collisionCells[index]->getNorthEastCell()){
					float length = std::sqrt(std::pow(northEastScreenPosition.x-nodeShape.getPosition().x, 2.0f) + std::pow(northEastScreenPosition.y-nodeShape.getPosition().y, 2.0f));
					sf::Vector2f size = sf::Vector2f(std::max(length,1.0f), 1.0f);
					edgeNorthEastShape.setSize(size);
					edgeNorthEastShape.setFillColor(sf::Color::Blue);
					window.draw(edgeNorthEastShape,&mRoadTexture);
				}
				if(collisionCells[index]->getNorthWestCell()){
					float length = std::sqrt(std::pow(northWestScreenPosition.x-nodeShape.getPosition().x, 2.0f) + std::pow(northWestScreenPosition.y-nodeShape.getPosition().y, 2.0f));
					sf::Vector2f size = sf::Vector2f(std::max(length,1.0f), 1.0f);
					edgeNorthWestShape.setSize(size);
					edgeNorthWestShape.setFillColor(sf::Color::White);
					window.draw(edgeNorthWestShape,&mRoadTexture);
				}
				if(collisionCells[index]->getSouthEastCell()){
					float length = std::sqrt(std::pow(southEastScreenPosition.x-nodeShape.getPosition().x, 2.0f) + std::pow(southEastScreenPosition.y-nodeShape.getPosition().y, 2.0f));
					sf::Vector2f size = sf::Vector2f(std::max(length,1.0f), 1.0f);
					edgeSouthEastShape.setSize(size);
					edgeSouthEastShape.setFillColor(sf::Color::Magenta);
					window.draw(edgeSouthEastShape,&mRoadTexture);
				}
				if(collisionCells[index]->getSouthWestCell()){
					float length = std::sqrt(std::pow(southWestScreenPosition.x-nodeShape.getPosition().x, 2.0f) + std::pow(southWestScreenPosition.y-nodeShape.getPosition().y, 2.0f));
					sf::Vector2f size = sf::Vector2f(std::max(length,1.0f), 1.0f);
					edgeSouthWestShape.setSize(size);
					edgeSouthWestShape.setFillColor(sf::Color::Black);
					window.draw(edgeSouthWestShape,&mRoadTexture);
				}

				nodeShape.move(sf::Vector2f(-2.5f,-2.5f));
				window.draw(nodeShape,&mRoadTexture);
			}
		}
	}
#endif
}

void District::teardown() {
    this->unlinkNeighbouringNavMeshes();
    this->removeResidentPooledPlayEntities();
}

#pragma Placing elements on the district

void District::setTile(int floor, sf::Vector2f worldPosition, int newTileNumber, std::string tileCategory){
	sf::VertexArray* usingVertexArray;
	sf::Texture* usingTexture;
	int** usingTiles;

	if(tileCategory.compare( CATEGORY_INTERIOR ) == 0) {
		usingVertexArray = &mInteriorVertexArray;
		usingTexture = &mInteriorTexture;
		usingTiles = interiorTiles;
	} else {
		usingVertexArray = &mRoadVertexArray;
		usingTexture = &mRoadTexture;
		usingTiles = roadTiles;
	}

	int xIndex = worldPosition.x/METERES_PER_TILE;
	int yIndex = worldPosition.y/METERES_PER_TILE;

	int tileIndex = xIndex + yIndex*NUM_TILES_WIDE;
	usingTiles[floor][tileIndex] = newTileNumber;

	int tileNumber = usingTiles[floor][tileIndex];

	// find its position in the tileset texture
	int tu = tileNumber % (usingTexture->getSize().x / TILE_PIXEL_DIMENSIONS);
	int tv = tileNumber / (usingTexture->getSize().x / TILE_PIXEL_DIMENSIONS);

	// get a pointer to the current tile's quad
	sf::Vertex* quad = &(*usingVertexArray)[(tileIndex) * VERTICES_PER_QUAD];

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tu * TILE_PIXEL_DIMENSIONS, tv * TILE_PIXEL_DIMENSIONS);
	quad[1].texCoords = sf::Vector2f((tu + 1) * TILE_PIXEL_DIMENSIONS, tv * TILE_PIXEL_DIMENSIONS);
	quad[2].texCoords = sf::Vector2f((tu + 1) * TILE_PIXEL_DIMENSIONS, (tv + 1) * TILE_PIXEL_DIMENSIONS);
	quad[3].texCoords = sf::Vector2f(tu * TILE_PIXEL_DIMENSIONS, (tv + 1) * TILE_PIXEL_DIMENSIONS);

	this->setVisibleRegion(floor, this->position, this->dimensions);
}

void District::setWall(int floor, sf::Vector2f tilePosition, int wallOrientation) {
	int xIndex = tilePosition.x/METERES_PER_TILE;
	int yIndex = tilePosition.y/METERES_PER_TILE;

	int tileIndex = xIndex + yIndex*NUM_TILES_WIDE;

	Wall* wall = walls[floor][tileIndex];
	if(wall) {
		delete wall;
		walls[floor][tileIndex] = NULL;
	}

	sf::Vector2f worldPositionTileAligned;
	worldPositionTileAligned.x = (xIndex-NUM_TILES_WIDE/2)*METERES_PER_TILE;
	worldPositionTileAligned.y = (yIndex-NUM_TILES_TALL/2)*METERES_PER_TILE;

	sf::Vector2f worldOffset = Camera::screenToWorld(mOffset);
	sf::Vector2f worldTranslatedOffset( worldOffset.x + (NUM_TILES_WIDE*METERES_PER_TILE)/2.0f, worldOffset.y + (NUM_TILES_WIDE*METERES_PER_TILE)/2.0f);

	if(wallOrientation != Wall::WallOrientations::NONE) {
		walls[floor][tileIndex] = new Wall(mPlayContext, mBody, mStaticBody, wallOrientation, b2Vec2(worldPositionTileAligned.x + worldTranslatedOffset.x, worldPositionTileAligned.y + worldTranslatedOffset.y), floor);
	}

	this->setVisibleRegion(floor, this->position, this->dimensions);
}

void District::setPooledPlayEntityForDescriptor(PooledPlayEntityDescriptor* descriptor) {
    std::vector<PooledPlayEntityDescriptor*> entityDescriptors = floorToEntityDescriptorsMap.find(descriptor->getFloor())->second;
    floorToEntityDescriptorsMap.erase(descriptor->getFloor());
    entityDescriptors.push_back(descriptor);
    floorToEntityDescriptorsMap.insert(std::pair<int, std::vector<PooledPlayEntityDescriptor*> >(descriptor->getFloor(), entityDescriptors));

    EntityManager *entityManger = this->mPlayContext->getEntityManager();
    Factory *factory = this->mPlayContext->getFactory();
    
    if (entityManger->doesPersistentEntityExists(descriptor->getUniqueReferenceId()) == false) {
        PooledPlayEntity* entity = factory->pooledPlayEntityForDescriptor(this->getPlayContext(), descriptor);
        entityManger->addEntity(entity);
    }
}

void District::unsetPooledPlayEntityForPersistenceIdentifer(int persistenceIdentifer) {
    PooledPlayEntityDescriptor *descriptorToRemove = NULL;
    
    for (auto pair : floorToEntityDescriptorsMap ) {
        std::vector<PooledPlayEntityDescriptor*> entityDescriptors = pair.second;
        
        for (auto descriptor : entityDescriptors) {
            if (descriptor->getUniqueReferenceId() == persistenceIdentifer) {
                descriptorToRemove = descriptor;
            }
        }
        
    }
    
    if (descriptorToRemove) {
        int floor = descriptorToRemove->getFloor();
        auto entityDescriptors = floorToEntityDescriptorsMap.find(floor)->second;
        entityDescriptors.erase(std::remove(entityDescriptors.begin(), entityDescriptors.end(), descriptorToRemove),
                                entityDescriptors.end());
        floorToEntityDescriptorsMap.erase(floor);
        floorToEntityDescriptorsMap.insert(std::pair<int, std::vector<PooledPlayEntityDescriptor*> >(floor, entityDescriptors));
    }
}

void District::setNavigationCellOccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity) {
    sf::Vector2f sfWorldLocation = Camera::screenToWorld(mOffset);
    
    b2Vec2 districtTopLeftLocation(sfWorldLocation.x,sfWorldLocation.y);
    
    b2Vec2 relativeLocationToDistrict = location-districtTopLeftLocation;
    
    b2Vec2 navCellIndex(relativeLocationToDistrict.x/DISTRICT_WIDTH_IN_METERS*NAVIGATION_CELLS_PER_DISTRICT_WIDE,
                        relativeLocationToDistrict.y/DISTRICT_WIDTH_IN_METERS*NAVIGATION_CELLS_PER_DISTRICT_WIDE);
    
    NavigationCell *cell = collisionCellAtIndex(navCellIndex.x, navCellIndex.y);
    
    cell->occupyCell(playEntity);
    
    mCellsThatHaveBeenOccupied->insert(cell);
}

void District::setNavigationCellUnoccupiedAtLocationWithEntity(b2Vec2 location, PlayEntity* playEntity) {
    sf::Vector2f sfWorldLocation = Camera::screenToWorld(mOffset);
    
    b2Vec2 districtTopLeftLocation(sfWorldLocation.x,sfWorldLocation.y);
    
    b2Vec2 relativeLocationToDistrict = location-districtTopLeftLocation;
    
    b2Vec2 navCellIndex(relativeLocationToDistrict.x/DISTRICT_WIDTH_IN_METERS*NAVIGATION_CELLS_PER_DISTRICT_WIDE,
                        relativeLocationToDistrict.y/DISTRICT_WIDTH_IN_METERS*NAVIGATION_CELLS_PER_DISTRICT_WIDE);
    
    NavigationCell *cell = collisionCellAtIndex(navCellIndex.x, navCellIndex.y);
    
    cell->unoccupyCell(playEntity);
    
//    mCellsThatHaveBeenOccupied->insert(cell);
}

#pragma Saving and Loading

void District::saveToFile(std::string filePath){
    std::string jsonString = "{\n}"; //empty json
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if(in.good()){
        jsonString = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
    rapidjson::Document* jsonDoc = new rapidjson::Document();
    jsonDoc->Parse(jsonString.c_str());
    
    for(int floorIndex=0; floorIndex<FLOORS_PER_DISTRICT; floorIndex++) {
        rapidjson::Value floor(rapidjson::kObjectType);
        
        /* add stuff here */
        rapidjson::Value *outdoorTileValue = valueForTileSetAtFloor(jsonDoc, roadTiles, floorIndex);
        rapidjson::Value *interiorTileValue = valueForTileSetAtFloor(jsonDoc, interiorTiles, floorIndex);
        rapidjson::Value *wallsValue = valueForWallsAtFloor(jsonDoc, walls, floorIndex);
        floor.AddMember("outdoorTiles", *outdoorTileValue, jsonDoc->GetAllocator());
        floor.AddMember("interiorTiles", *interiorTileValue, jsonDoc->GetAllocator());
        floor.AddMember("walls", *wallsValue, jsonDoc->GetAllocator());
        
        rapidjson::Value pooledPlayedObjectsArray(rapidjson::kArrayType);
        std::vector<PooledPlayEntityDescriptor*> entityDescriptors = this->floorToEntityDescriptorsMap.find(floorIndex)->second;
        for(std::vector<PooledPlayEntityDescriptor*>::iterator it = entityDescriptors.begin();
            it != entityDescriptors.end();
            ++it)
        {
            PooledPlayEntityDescriptor* descriptor = *it;
            rapidjson::Value* value = descriptor->descriptorJsonObjectValue(jsonDoc);
            
            pooledPlayedObjectsArray.PushBack(*value, jsonDoc->GetAllocator());
        }
        floor.AddMember("pooledPlayEntityDescriptors", pooledPlayedObjectsArray, jsonDoc->GetAllocator());
        
        /* write the floor */
        const char* keyString = (std::string("floor_") + intToString(floorIndex)).c_str();
        
        if(jsonDoc->HasMember(keyString)){
            jsonDoc->RemoveMember(keyString);
        }
        
        rapidjson::Value keyStringValue;
        char tileKeyNameBuffer[100];
        int tileStringLength = sprintf(tileKeyNameBuffer, "%s", keyString);
        keyStringValue.SetString(tileKeyNameBuffer, tileStringLength, jsonDoc->GetAllocator());
        jsonDoc->AddMember(keyStringValue, floor, jsonDoc->GetAllocator());
    }
    
    FILE* file = fopen((filePath).c_str() , "w");
    if (file){
        rapidjson::FileStream fileStream(file);
        rapidjson::PrettyWriter<rapidjson::FileStream> writer(fileStream);
        jsonDoc->Accept(writer);
        
        fclose(file);
    }
    
    delete jsonDoc;
}

void District::loadFromFile(std::string filePath){
	std::string jsonString = "{\n}"; //empty json

	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if(in.good()){
		jsonString = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}

	rapidjson::Document* jsonDoc = new rapidjson::Document();
	jsonDoc->Parse(jsonString.c_str());
    
    for(int floorIndex=0; floorIndex<1/*FLOORS_PER_DISTRICT*/; floorIndex++) {
        const char* keyString = (std::string("floor_") + intToString(floorIndex)).c_str();
        const rapidjson::Value& floor = (*jsonDoc)[keyString];
        
        const rapidjson::Value& outdoorTileValue = floor["outdoorTiles"];
        populateTilesetAtFloorFromValue(roadTiles, floorIndex, outdoorTileValue);
        const rapidjson::Value& interiorTilesValue = floor["interiorTiles"];
        populateTilesetAtFloorFromValue(interiorTiles, floorIndex, interiorTilesValue);
        const rapidjson::Value& wallsValue = floor["walls"];
        populateWallsAtFloorFromValue(floorIndex, wallsValue);
        
        const rapidjson::Value& pooledPlayEntityDescriptors = floor["pooledPlayEntityDescriptors"];
        populateDistrictWithEntitesFromJsonValue(pooledPlayEntityDescriptors);
    }
    
	delete jsonDoc;
}

void District::setVisibleRegion(int floor, sf::Vector2f position, sf::Vector2f dimensions){
    this->position = position;
    this->dimensions = dimensions;
    sf::Vector2i indexPositionTopLeft     = District::worldSpaceToTileIndexSpace(sf::Vector2f(position.x-dimensions.x/2.0f,position.y-dimensions.y/2.0f));
    sf::Vector2i indexPositionBottomRight = District::worldSpaceToTileIndexSpace(sf::Vector2f(position.x+dimensions.x/2.0f,position.y+dimensions.y/2.0f));
    this->buildDrawableRegion(floor, indexPositionTopLeft, indexPositionBottomRight);
}

#pragma mark - Private

void District::buildDrawableRegion(int floor, sf::Vector2i topLeft, sf::Vector2i bottomRight) {
    //adds one more tile to the right and to the bottom, otherwise they will jump into existence
    bottomRight.x++;
    
    sf::Vector2i actualTileRegionTopLeft(mIndexLocation.x*NUM_TILES_WIDE, mIndexLocation.y*NUM_TILES_TALL);
    sf::Vector2i actualTileRegionBottomRight((mIndexLocation.x+1)*NUM_TILES_WIDE, (mIndexLocation.y+1)*NUM_TILES_TALL);
    
    //the wanted values are out of range so return
    if(topLeft.x >= actualTileRegionBottomRight.x){
        mRoadVertexArray.resize(0);
        mInteriorVertexArray.resize(0);
        return;
    } else if(topLeft.y >= actualTileRegionBottomRight.y){
        mRoadVertexArray.resize(0);
        mInteriorVertexArray.resize(0);
        return;
    } else if(bottomRight.x < actualTileRegionTopLeft.x){
        mRoadVertexArray.resize(0);
        mInteriorVertexArray.resize(0);
        return;
    } else if(bottomRight.y < actualTileRegionTopLeft.y){
        mRoadVertexArray.resize(0);
        mInteriorVertexArray.resize(0);
        return;
    }
    
    if(topLeft.x < actualTileRegionTopLeft.x){
        topLeft.x = actualTileRegionTopLeft.x;
    }
    if(topLeft.y < actualTileRegionTopLeft.y){
        topLeft.y = actualTileRegionTopLeft.y;
    }
    if(bottomRight.x > actualTileRegionBottomRight.x-1){
        bottomRight.x = actualTileRegionBottomRight.x-1;
    }
    if(bottomRight.y > actualTileRegionBottomRight.y-1){
        bottomRight.y = actualTileRegionBottomRight.y-1;
    }
    
    topLeft.x  = mod(topLeft.x, NUM_TILES_WIDE);
    topLeft.y  = mod(topLeft.y, NUM_TILES_TALL);
    bottomRight.x  = mod(bottomRight.x, NUM_TILES_WIDE);
    bottomRight.y  = mod(bottomRight.y, NUM_TILES_TALL);
    //adds one more tile to the right and to the bottom, otherwise they will jump into existence
    bottomRight.x++;
    bottomRight.y++;
    
    mRoadVertexArray.resize((bottomRight.x-topLeft.x)*(bottomRight.y-topLeft.y) * VERTICES_PER_QUAD);
    mInteriorVertexArray.resize((bottomRight.x-topLeft.x)*(bottomRight.y-topLeft.y) * VERTICES_PER_QUAD);
    
    int xiters = 0;
    int yiters = 0;
    
    for (int i = topLeft.x; i < bottomRight.x; ++i){ //x
        xiters++;
        yiters=0;
        for (int j = topLeft.y; j < bottomRight.y; ++j) { //y
            yiters++;
            // get the current tile number
            int tileNumber = roadTiles[floor][i + j*NUM_TILES_WIDE];
            
            // find its position in the tileset texture
            float tu = tileNumber % (mRoadTexture.getSize().x / TILE_PIXEL_DIMENSIONS);
            float tv = tileNumber / (mRoadTexture.getSize().x / TILE_PIXEL_DIMENSIONS);
            
            // get a pointer to the current tile's quad
            int iIndex = i-topLeft.x;
            int jIndex = j-topLeft.y;
            sf::Vertex* quad = &mRoadVertexArray[(iIndex + jIndex * (bottomRight.x-topLeft.x)) * VERTICES_PER_QUAD];
            // define its 4 corners
            sf::FloatRect tilePixelBounds = this->getTilePixelBounds(i, j);
            quad[0].position = sf::Vector2f(tilePixelBounds.left,  tilePixelBounds.top);
            quad[1].position = sf::Vector2f(tilePixelBounds.width, tilePixelBounds.top);
            quad[2].position = sf::Vector2f(tilePixelBounds.width, tilePixelBounds.height);
            quad[3].position = sf::Vector2f(tilePixelBounds.left,  tilePixelBounds.height);
            
            //define its 4 texture coordinates
            quad[0].texCoords = District::tilePixelVertexPositions[(int)tv][(int)tu];  //top left
            quad[1].texCoords = District::tilePixelVertexPositions[(int)tv][(int)tu+1]; //top right
            quad[2].texCoords = District::tilePixelVertexPositions[(int)tv+1][(int)tu+1]; //bottom right
            quad[3].texCoords = District::tilePixelVertexPositions[(int)tv+1][(int)tu];  //bottom left
            quad[0].texCoords.x+=0.01f;
            quad[0].texCoords.y+=0.01f;
            quad[1].texCoords.y+=0.01f;
            quad[3].texCoords.x+=0.01f;
        }
    }
    
    //road to draw
    this->buildVertexArrayWithTexture(floor, topLeft, bottomRight, mRoadVertexArray, roadTiles, &mRoadTexture);
    
    //interior to build
    this->buildVertexArrayWithTexture(floor, topLeft, bottomRight, mInteriorVertexArray, interiorTiles, &mInteriorTexture);
    
    //walls to draw
    mWallsToDraw.clear();
    
    for(int x=topLeft.x; x<bottomRight.x; x++) {
        for(int y=topLeft.y; y<bottomRight.y; y++){
            Wall *wall = walls[floor][y*NUM_TILES_WIDE + x];
            if(wall) {
                mWallsToDraw.push_back(wall);
            }
        }
    }
    
}

void District::buildVertexArrayWithTexture(int floor, sf::Vector2i topLeft, sf::Vector2i bottomRight, sf::VertexArray& vertexArray, int** tiles, sf::Texture* texture) {
    vertexArray.resize((bottomRight.x-topLeft.x)*(bottomRight.y-topLeft.y) * VERTICES_PER_QUAD);
    
    int xiters = 0;
    int yiters = 0;
    
    for (int i = topLeft.x; i < bottomRight.x; ++i){ //x
        xiters++;
        yiters=0;
        for (int j = topLeft.y; j < bottomRight.y; ++j) { //y
            yiters++;
            // get the current tile number
            int tileNumber = tiles[floor][i + j*NUM_TILES_WIDE];
            
            // find its position in the tileset texture
            float tu = tileNumber % (texture->getSize().x / TILE_PIXEL_DIMENSIONS);
            float tv = tileNumber / (texture->getSize().x / TILE_PIXEL_DIMENSIONS);
            
            // get a pointer to the current tile's quad
            int iIndex = i-topLeft.x;
            int jIndex = j-topLeft.y;
            sf::Vertex* quad = &vertexArray[(iIndex + jIndex * (bottomRight.x-topLeft.x)) * VERTICES_PER_QUAD];
            // define its 4 corners
            sf::FloatRect tilePixelBounds = this->getTilePixelBounds(i, j);
            quad[0].position = sf::Vector2f(tilePixelBounds.left,  tilePixelBounds.top);
            quad[1].position = sf::Vector2f(tilePixelBounds.width, tilePixelBounds.top);
            quad[2].position = sf::Vector2f(tilePixelBounds.width, tilePixelBounds.height);
            quad[3].position = sf::Vector2f(tilePixelBounds.left,  tilePixelBounds.height);
            
            //define its 4 texture coordinates
            quad[0].texCoords = District::tilePixelVertexPositions[(int)tv][(int)tu];  //top left
            quad[1].texCoords = District::tilePixelVertexPositions[(int)tv][(int)tu+1]; //top right
            quad[2].texCoords = District::tilePixelVertexPositions[(int)tv+1][(int)tu+1]; //bottom right
            quad[3].texCoords = District::tilePixelVertexPositions[(int)tv+1][(int)tu];  //bottom left
            quad[0].texCoords.x+=0.01f;
            quad[0].texCoords.y+=0.01f;
            quad[1].texCoords.y+=0.01f;
            quad[3].texCoords.x+=0.01f;
        }
    }
    
    //std::cout << "end " << "build vertex array" << std::endl;
}

sf::Vector2i District::worldSpaceToTileIndexSpace(sf::Vector2f position){
	int x = std::floor(position.x/District::METERES_PER_TILE + NUM_TILES_WIDE/2);
	int y = std::floor(position.y/District::METERES_PER_TILE + NUM_TILES_WIDE/2);
	return sf::Vector2i(x,y);
}

int District::indexFromIndexTable(int index, District::Directions direction){
	int xcoord = ((index) % (NAVIGATION_CELLS_PER_DISTRICT_WIDE));
	int ycoord = ((index) / NAVIGATION_CELLS_PER_DISTRICT_WIDE);
	xcoord++;
	ycoord++;
	if(xcoord>NAVIGATION_CELLS_PER_DISTRICT_WIDE) {
		xcoord = 0;
		ycoord++;
	}

	switch(direction) {
		case District::Directions::EAST: xcoord+=1; break;
		case District::Directions::WEST: xcoord-=1; break;
		case District::Directions::NORTH: ycoord-=1; break;
		case District::Directions::SOUTH: ycoord+=1; break;
		case District::Directions::NORTHEAST: xcoord+=1; ycoord-=1; break;
		case District::Directions::NORTHWEST: xcoord-=1; ycoord-=1; break;
		case District::Directions::SOUTHEAST: xcoord+=1; ycoord+=1; break;
		case District::Directions::SOUTHWEST: xcoord-=1; ycoord+=1; break;
		case District::Directions::NONE: break;
	}
	return District::indexTable[ycoord][xcoord];
}

bool isInCollisionIndexBounds(int index){
	if(index >= 0 && index < NAVIGATION_CELLS_PER_DISTRICT_TALL*NAVIGATION_CELLS_PER_DISTRICT_WIDE){
		return true;
	}
	return false;
}

void District::setupCollisionCells(){
	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
		for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
			int index = indexFromIndexTable(x+y*NAVIGATION_CELLS_PER_DISTRICT_WIDE, Directions::NONE);
			int eastIndex = indexFromIndexTable(index,Directions::EAST);
			int westIndex = indexFromIndexTable(index,Directions::WEST);
			int northIndex = indexFromIndexTable(index,Directions::NORTH);
			int southIndex = indexFromIndexTable(index,Directions::SOUTH);
			int northEastIndex = indexFromIndexTable(index,Directions::NORTHEAST);
			int northWestIndex = indexFromIndexTable(index,Directions::NORTHWEST);
			int southEastIndex = indexFromIndexTable(index,Directions::SOUTHEAST);
			int southWestIndex = indexFromIndexTable(index,Directions::SOUTHWEST);
			NavigationCell * centerCell = (isInCollisionIndexBounds(index)) ? collisionCells[index] : NULL;
			NavigationCell * eastCell = (isInCollisionIndexBounds(eastIndex)) ? collisionCells[eastIndex] : NULL;
			NavigationCell * westCell = (isInCollisionIndexBounds(westIndex)) ? collisionCells[westIndex] : NULL;
			NavigationCell * northCell = (isInCollisionIndexBounds(northIndex)) ? collisionCells[northIndex] : NULL;
			NavigationCell * southCell = (isInCollisionIndexBounds(southIndex)) ? collisionCells[southIndex] : NULL;
			NavigationCell * northEast = (isInCollisionIndexBounds(northEastIndex)) ? collisionCells[northEastIndex] : NULL;
			NavigationCell * northWest = (isInCollisionIndexBounds(northWestIndex)) ? collisionCells[northWestIndex] : NULL;
			NavigationCell * southEast = (isInCollisionIndexBounds(southEastIndex)) ? collisionCells[southEastIndex] : NULL;
			NavigationCell * southWest = (isInCollisionIndexBounds(southWestIndex)) ? collisionCells[southWestIndex] : NULL;
			centerCell->setEastCell(eastCell);
			centerCell->setWestCell(westCell);
			centerCell->setNorthCell(northCell);
			centerCell->setSouthCell(southCell);
			centerCell->setNorthEastCell(northEast);
			centerCell->setNorthWestCell(northWest);
			centerCell->setSouthEastCell(southEast);
			centerCell->setSouthWestCell(southWest);
		}
	}
}

NavigationCell * District::collisionCellAtIndex(int x, int y){
	return collisionCells[x+y*NAVIGATION_CELLS_PER_DISTRICT_WIDE];
}

std::vector<NavigationCell*> District::getNavigationCellsList() {
    std::vector<NavigationCell*> returnList;
    
    for (int i = 0; i<NAVIGATION_CELLS_PER_DISTRICT_WIDE*NAVIGATION_CELLS_PER_DISTRICT_TALL; i++) {
        returnList.push_back(collisionCells[i]);
    }
    
    return returnList;
}

std::vector<NavigationCell *> District::getCollisionCellsTopEdge() {
	std::vector<NavigationCell *> vec;
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		vec.push_back( this->collisionCellAtIndex(x,0) );
	}
	return vec;
}

std::vector<NavigationCell *> District::getCollisionCellsBottomEdge() {
	std::vector<NavigationCell *> vec;
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		vec.push_back( this->collisionCellAtIndex(x,NAVIGATION_CELLS_PER_DISTRICT_TALL-1) );
	}
	return vec;
}

std::vector<NavigationCell *> District::getCollisionCellsLeftEdge() {
	std::vector<NavigationCell *> vec;
	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
		vec.push_back( this->collisionCellAtIndex(0,y) );
	}
	return vec;
}
std::vector<NavigationCell *> District::getCollisionCellsRightEdge() {
	std::vector<NavigationCell *> vec;
	for(int y=0; y<NAVIGATION_CELLS_PER_DISTRICT_TALL; y++){
		vec.push_back( this->collisionCellAtIndex(NAVIGATION_CELLS_PER_DISTRICT_WIDE-1,y) );
	}
	return vec;
}
NavigationCell * District::getNorthEastCornerCollisionCell() {
	return this->collisionCellAtIndex(NAVIGATION_CELLS_PER_DISTRICT_WIDE-1, 0);
}
NavigationCell * District::getNorthWestCornerCollisionCell() {
	return this->collisionCellAtIndex(0, 0);
}
NavigationCell * District::getSouthEastCornerCollisionCell() {
	return this->collisionCellAtIndex(NAVIGATION_CELLS_PER_DISTRICT_WIDE-1, NAVIGATION_CELLS_PER_DISTRICT_TALL-1);
}
NavigationCell * District::getSouthWestCornerCollisionCell() {
	return this->collisionCellAtIndex(0, NAVIGATION_CELLS_PER_DISTRICT_TALL-1);
}

/* linkNorth
 *
 * Links the north edge of this district nav mesh to the south section of northDistrict's navmesh
 */
void District::linkNorth(District* northDistrict) {
	std::vector<NavigationCell *> southEdge = northDistrict->getCollisionCellsBottomEdge();
	std::vector<NavigationCell *> northEdge = this->getCollisionCellsTopEdge();
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){ //verticals
		northEdge[x]->setNorthCell( southEdge[x] );
		southEdge[x]->setSouthCell( northEdge[x] );
	}
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE-1; x++){ //diagonals
		northEdge[x]->setNorthEastCell( southEdge[x+1] );
		southEdge[x+1]->setSouthWestCell( northEdge[x] );

		southEdge[x]->setSouthEastCell( northEdge[x+1] );
		northEdge[x+1]->setNorthWestCell( southEdge[x] );
	}
}
void District::linkSouth(District* southDistrict) {
	std::vector<NavigationCell *> northEdge = southDistrict->getCollisionCellsTopEdge();
	std::vector<NavigationCell *> southEdge = this->getCollisionCellsBottomEdge();
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){ //verticals
		southEdge[x]->setSouthCell( northEdge[x] );
		northEdge[x]->setNorthCell( southEdge[x] );
	}
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE-1; x++){ //diagonals
		northEdge[x]->setNorthEastCell( southEdge[x+1] );
		southEdge[x+1]->setSouthWestCell( northEdge[x] );

		southEdge[x]->setSouthEastCell( northEdge[x+1] );
		northEdge[x+1]->setNorthWestCell( southEdge[x] );
	}
}
void District::linkEast(District* eastDistrict) {
	std::vector<NavigationCell *> westEdge = eastDistrict->getCollisionCellsLeftEdge();
	std::vector<NavigationCell *> eastEdge = this->getCollisionCellsRightEdge();
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		eastEdge[x]->setEastCell( westEdge[x] );
		westEdge[x]->setWestCell( eastEdge[x] );
	}
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE-1; x++){ //diagonals
		eastEdge[x]->setSouthEastCell( westEdge[x+1] );
		westEdge[x+1]->setNorthWestCell( eastEdge[x] );

		westEdge[x]->setSouthWestCell( eastEdge[x+1] );
		eastEdge[x+1]->setNorthEastCell( westEdge[x] );
	}
}
void District::linkWest(District* westDistrict) {
	std::vector<NavigationCell *> westEdge = this->getCollisionCellsLeftEdge();
	std::vector<NavigationCell *> eastEdge = westDistrict->getCollisionCellsRightEdge();
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE; x++){
		westEdge[x]->setWestCell( eastEdge[x] );
		eastEdge[x]->setEastCell( westEdge[x] );
	}
	for(int x=0; x<NAVIGATION_CELLS_PER_DISTRICT_WIDE-1; x++){ //diagonals
		eastEdge[x]->setSouthEastCell( westEdge[x+1] );
		westEdge[x+1]->setNorthWestCell( eastEdge[x] );

		westEdge[x]->setSouthWestCell( eastEdge[x+1] );
		eastEdge[x+1]->setNorthEastCell( westEdge[x] );
	}
}
void District::linkNorthEast(District* northEastDistrict) {
	NavigationCell * northEastCell = this->getNorthEastCornerCollisionCell();
	NavigationCell * southWestCell = northEastDistrict->getSouthWestCornerCollisionCell();
	northEastCell->setNorthEastCell( southWestCell );
	southWestCell->setSouthWestCell( northEastCell );
}
void District::linkNorthWest(District* northWestDistrict) {
	NavigationCell * northWestCell = this->getNorthWestCornerCollisionCell();
	NavigationCell * southEastCell = northWestDistrict->getSouthEastCornerCollisionCell();
	northWestCell->setNorthWestCell( southEastCell );
	southEastCell->setSouthEastCell( northWestCell );
}
void District::linkSouthEast(District* southEastDistrict) {
	NavigationCell * southEastCell = this->getSouthEastCornerCollisionCell();
	NavigationCell * northWestCell = southEastDistrict->getNorthWestCornerCollisionCell();
	southEastCell->setSouthEastCell( northWestCell );
	northWestCell->setNorthWestCell( southEastCell );
}
void District::linkSouthWest(District* southWestDistrict){
	NavigationCell * southWestCell = this->getSouthWestCornerCollisionCell();
	NavigationCell * northEastCell = southWestDistrict->getNorthEastCornerCollisionCell();
	southWestCell->setSouthWestCell( northEastCell );
	northEastCell->setNorthEastCell( southWestCell );
}

/* unlinkNeightbouringNavMeshes
 *
 * Unlink's the neighbouring districts' nav meshes from this district's nav mesh
 */
void District::unlinkNeighbouringNavMeshes(){
	std::vector<NavigationCell *> topEdge = getCollisionCellsTopEdge();
	std::vector<NavigationCell *> bottomEdge = getCollisionCellsBottomEdge();
	std::vector<NavigationCell *> leftEdge = getCollisionCellsLeftEdge();
	std::vector<NavigationCell *> rightEdge = getCollisionCellsRightEdge();

	std::vector<NavigationCell *> borderCells;
	borderCells.insert(borderCells.end(), topEdge.begin(), topEdge.end());
	borderCells.insert(borderCells.end(), bottomEdge.begin(), bottomEdge.end());
	borderCells.insert(borderCells.end(), leftEdge.begin(), leftEdge.end());
	borderCells.insert(borderCells.end(), rightEdge.begin(), rightEdge.end());

	for (NavigationCell * cell : borderCells){
		if(cell->getNorthCell()) {
			cell->getNorthCell()->setSouthCell(NULL);
		}
		if(cell->getSouthCell()) {
			cell->getSouthCell()->setNorthCell(NULL);
		}
		if(cell->getEastCell()) {
			cell->getEastCell()->setWestCell(NULL);
		}
		if(cell->getWestCell()) {
			cell->getWestCell()->setEastCell(NULL);
		}

		if(cell->getNorthEastCell()) {
			cell->getNorthEastCell()->setSouthWestCell(NULL);
		}
		if(cell->getNorthWestCell()) {
			cell->getNorthWestCell()->setSouthEastCell(NULL);
		}
		if(cell->getSouthEastCell()) {
			cell->getSouthEastCell()->setNorthWestCell(NULL);
		}
		if(cell->getSouthWestCell()) {
			cell->getSouthWestCell()->setNorthEastCell(NULL);
		}
	}
}

void District::removeResidentPooledPlayEntities() {
    PooledPlayEntityFinderQueryCallback callback;
    b2AABB aabb;
    aabb.lowerBound = this->getBody()->GetPosition() - b2Vec2(METERES_PER_TILE*(NUM_TILES_WIDE/2.0f),METERES_PER_TILE*(NUM_TILES_TALL/2.0f));
    aabb.upperBound = this->getBody()->GetPosition() + b2Vec2(METERES_PER_TILE*(NUM_TILES_WIDE/2.0f),METERES_PER_TILE*(NUM_TILES_TALL/2.0f));
    mPlayContext->getWorld()->QueryAABB(&callback, aabb);
    std::vector<PooledPlayEntity*> entities = callback.getPooledPlayEntitiesFound();
    
    for (PooledPlayEntity* pooledPlayEntity : entities ) {
        //TODO: figure out why pooledPlayEntity->getBody() returns the wrong body...
            //It would be better to only remove PooledPlayEntities that have their center in a district
            // rather than removing it if it merely touches a district.
            // I might be requiring too much detail, but I should still be able to retrieve a body...
        
//        bool isEntityCenterInDistrict = false;
//        
//        if (pooledPlayEntity->getBody()->GetPosition().x > aabb.upperBound.x &&
//            pooledPlayEntity->getBody()->GetPosition().y > aabb.upperBound.y &&
//            pooledPlayEntity->getBody()->GetPosition().x < aabb.lowerBound.x &&
//            pooledPlayEntity->getBody()->GetPosition().y < aabb.lowerBound.y)
//        {
//            isEntityCenterInDistrict = true;
//        }
//        
//        if (isEntityCenterInDistrict) {
            pooledPlayEntity->setIsStillPartOfGame(false);
//        }
    }
}

#pragma mark - PlayEntity abstract methods

Animation* District::newBulletStruckAnimation() {
	return NULL;
}

void District::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void District::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}
