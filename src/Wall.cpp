#include "Wall.hpp"

#include <iostream>
#

#include "CollisionListener.hpp"
#include "Camera.hpp"
#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Animal.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Rect.hpp>

#define VERTEX_COUNT 4

#define CELL_HEIGHT 2.0f
#define CELL_WIDTH CELL_HEIGHT

#define STATIC_WALL_THICKNESS 0.2f
#define DYNAMIC_WALL_THICKNESS 0.07f

#define CELL_TOP_STATIC             bodyOffsetWorldPosition.y
#define CELL_BOTTOM_STATIC          bodyOffsetWorldPosition.y + CELL_HEIGHT
#define CELL_LEFT_STATIC            bodyOffsetWorldPosition.x
#define CELL_RIGHT_STATIC           bodyOffsetWorldPosition.x + CELL_WIDTH

#define CELL_TOP_STATIC_WALL        bodyOffsetWorldPosition.y + STATIC_WALL_THICKNESS
#define CELL_BOTTOM_STATIC_WALL     bodyOffsetWorldPosition.y + CELL_HEIGHT - STATIC_WALL_THICKNESS
#define CELL_LEFT_STATIC_WALL       bodyOffsetWorldPosition.x + STATIC_WALL_THICKNESS
#define CELL_RIGHT_STATIC_WALL      bodyOffsetWorldPosition.x + CELL_WIDTH - STATIC_WALL_THICKNESS

#define CELL_TOP_DYNAMIC            bodyOffsetWorldPosition.y + DYNAMIC_WALL_THICKNESS
#define CELL_BOTTOM_DYNAMIC         bodyOffsetWorldPosition.y + CELL_HEIGHT - DYNAMIC_WALL_THICKNESS
#define CELL_LEFT_DYNAMIC           bodyOffsetWorldPosition.x + DYNAMIC_WALL_THICKNESS
#define CELL_RIGHT_DYNAMIC          bodyOffsetWorldPosition.x + CELL_WIDTH - DYNAMIC_WALL_THICKNESS

#define CELL_TOP_DYNAMIC_WALL       bodyOffsetWorldPosition.y + DYNAMIC_WALL_THICKNESS*2
#define CELL_BOTTOM_DYNAMIC_WALL    bodyOffsetWorldPosition.y + CELL_HEIGHT - DYNAMIC_WALL_THICKNESS*2
#define CELL_LEFT_DYNAMIC_WALL      bodyOffsetWorldPosition.x + DYNAMIC_WALL_THICKNESS*2
#define CELL_RIGHT_DYNAMIC_WALL     bodyOffsetWorldPosition.x + CELL_WIDTH - DYNAMIC_WALL_THICKNESS*2

//    ____________________
//   |_|________________|_|
//   |.|                |.|
//   |.|                |.|
//   |.|                |.|
//   |.|                |.|
//   |.|                |.|
//   |.|                |.|
//   |_|________________|_|
//   |_|________________|_|

Wall::Wall(PlayContext* playContext, b2Body* ownerBody, b2Body* ownerStaticBody, int wallOrientation, b2Vec2 worldPosition, int floor)
: PlayEntity() {
	mPlayContext = playContext;
	mBody = ownerBody;
    mStaticBody = ownerStaticBody;
	mWallOrientation = wallOrientation;

	mCurrentFloor = floor;

	int wallImageOffsetX = mod(wallOrientation,10);
	int wallImageOffsetY = wallOrientation/10;

	sf::Image* wallImages = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::wallTilemap_png);
	sf::IntRect area;
	area.left = wallImageOffsetX*100;
	area.top = wallImageOffsetY*100;
	area.width = 100;
	area.height = 100;
	mWallTexture.loadFromImage(*wallImages, area);

	mSprite.setTexture(mWallTexture, true);
	sf::Vector2f screenPosition = Camera::worldToScreen(sf::Vector2f(worldPosition.x,worldPosition.y));
	mSprite.setPosition(screenPosition);


	mWallOrientation = wallOrientation;

	mStaticFixtures.clear();
	mDynamicFixtures.clear();

	b2Vec2 bodyOffsetWorldPosition = worldPosition - mBody->GetPosition();

	if(mBody != NULL) {
		b2Vec2 staticVertices[VERTEX_COUNT];
        b2Vec2 dynamicVertices[VERTEX_COUNT];

		switch(wallOrientation) {
			case Wall::WallOrientations::RIGHT : {
				staticVertices[0].Set(CELL_RIGHT_STATIC_WALL,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC,       CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC,       CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_RIGHT_STATIC_WALL,  CELL_BOTTOM_STATIC);
                
                dynamicVertices[0].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_TOP_DYNAMIC);    //top-left
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC,       CELL_TOP_DYNAMIC);    //top-right
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC,       CELL_BOTTOM_DYNAMIC); //bottom-right
                dynamicVertices[3].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_BOTTOM_DYNAMIC); //bottom-left

				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;
                
			case Wall::WallOrientations::TOP : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC_WALL);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC_WALL);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC_WALL);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC_WALL);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;

			case Wall::WallOrientations::LEFT : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  	 CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_LEFT_STATIC_WALL, CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_LEFT_STATIC_WALL, CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  	 CELL_BOTTOM_STATIC);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  	   CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_LEFT_DYNAMIC_WALL, CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_LEFT_DYNAMIC_WALL, CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,      CELL_BOTTOM_DYNAMIC);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;
                
			case Wall::WallOrientations::BOTTOM : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC_WALL);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC_WALL);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC);
                
                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC);

				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;

			case Wall::WallOrientations::TOP_RIGHT : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC_WALL);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC_WALL);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC_WALL);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC_WALL);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);

				staticVertices[0].Set(CELL_RIGHT_STATIC_WALL,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC,       CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC,       CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_RIGHT_STATIC_WALL,  CELL_BOTTOM_STATIC);
                
                dynamicVertices[0].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC,       CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC,       CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_BOTTOM_DYNAMIC);

				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;

			case Wall::WallOrientations::TOP_LEFT : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_TOP_STATIC_WALL);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_TOP_STATIC_WALL);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_TOP_DYNAMIC_WALL);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_TOP_DYNAMIC_WALL);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);

				staticVertices[0].Set(CELL_LEFT_STATIC,  		        CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_LEFT_STATIC_WALL, CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_LEFT_STATIC_WALL, CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  		        CELL_BOTTOM_STATIC);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,       CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_LEFT_DYNAMIC_WALL,  CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_LEFT_DYNAMIC_WALL,  CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  		CELL_BOTTOM_DYNAMIC);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;

			case Wall::WallOrientations::BOTTOM_RIGHT : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC_WALL);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC_WALL);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);

				staticVertices[0].Set(CELL_RIGHT_STATIC_WALL,  CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_RIGHT_STATIC,    				  CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_RIGHT_STATIC,    				  CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_RIGHT_STATIC_WALL,  CELL_BOTTOM_STATIC);
                
                dynamicVertices[0].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC,    				  CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC,    				  CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_RIGHT_DYNAMIC_WALL,  CELL_BOTTOM_DYNAMIC);

				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;
                
			case Wall::WallOrientations::BOTTOM_LEFT : {
				staticVertices[0].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC_WALL);
				staticVertices[1].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC_WALL);
				staticVertices[2].Set(CELL_RIGHT_STATIC, CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  CELL_BOTTOM_STATIC);
                
                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[1].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC_WALL);
                dynamicVertices[2].Set(CELL_RIGHT_DYNAMIC, CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  CELL_BOTTOM_DYNAMIC);

				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);

				staticVertices[0].Set(CELL_LEFT_STATIC,  		           CELL_TOP_STATIC);
				staticVertices[1].Set(CELL_LEFT_STATIC_WALL,    CELL_TOP_STATIC);
				staticVertices[2].Set(CELL_LEFT_STATIC_WALL,    CELL_BOTTOM_STATIC);
				staticVertices[3].Set(CELL_LEFT_STATIC,  		           CELL_BOTTOM_STATIC);

                dynamicVertices[0].Set(CELL_LEFT_DYNAMIC,  		           CELL_TOP_DYNAMIC);
                dynamicVertices[1].Set(CELL_LEFT_DYNAMIC_WALL,    CELL_TOP_DYNAMIC);
                dynamicVertices[2].Set(CELL_LEFT_DYNAMIC_WALL,    CELL_BOTTOM_DYNAMIC);
                dynamicVertices[3].Set(CELL_LEFT_DYNAMIC,  		           CELL_BOTTOM_DYNAMIC);
                
				this->addFixture(staticVertices, dynamicVertices, VERTEX_COUNT);
			} break;
                
			default : {
				//Wall::WallOrientations::NONE and errors
			}
		}
	}
    
    this->setup(playContext, floor);
}

Wall::~Wall() {
    for (std::vector<b2Fixture*>::iterator it = this->mStaticFixtures.begin() ; it != this->mStaticFixtures.end(); ++it) {
        b2Fixture *fixture = *it;
        
        mStaticBody->DestroyFixture(fixture);
    }
    
    for (std::vector<b2Fixture*>::iterator it = this->mDynamicFixtures.begin() ; it != this->mDynamicFixtures.end(); ++it) {
        b2Fixture *fixture = *it;
        
        mBody->DestroyFixture(fixture);
    }
    
    mStaticFixtures.clear();
    mDynamicFixtures.clear();
}

void Wall::update(float timeStep) {
	//Does not call PlayEntity update because that repositions the texture to the center of the body (which is the center of the district, not the centre of the wall)

	for(unsigned int i=0; i<mBulletStrikeAnimations.size(); i++){
		Animation* bulletStrikeAnimation = mBulletStrikeAnimations.at(i);
		if(bulletStrikeAnimation->hasCompletedAnimation()) {
			delete mBulletStrikeAnimations.at(i);
			mBulletStrikeAnimations.erase(mBulletStrikeAnimations.begin()+i);
			mBulletStrikeSprites.erase(mBulletStrikeSprites.begin()+i);
			i--;
		}

	}
	for(unsigned int i=0; i<mBulletStrikeAnimations.size(); i++){
		Animation* bulletStrikeAnimation = mBulletStrikeAnimations.at(i);
		bulletStrikeAnimation->update(timeStep);

		mBulletStrikeSprites.at(i).setTexture(*bulletStrikeAnimation->getCurrentFrame(), true);

	}
}

void Wall::draw(sf::RenderTarget &window) {
    //calling super draw will center the sprite on it's body.
    // In this case the body is the district, which would be incorrect.
    
    window.draw(mSprite);
    
    for(sf::Sprite sprite : mBulletStrikeSprites) {
        window.draw(sprite);
    }
}

void Wall::addFixture(b2Vec2 staticVertices[], b2Vec2 dynamicVertices[], int vertexCount) {
	b2PolygonShape staticBodyShape;
	staticBodyShape.Set(staticVertices, vertexCount);
	b2FixtureDef staticFixtureDef;
    staticFixtureDef.friction = 0.0f;
	staticFixtureDef.shape = &staticBodyShape;
	staticFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	staticFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2Fixture* staticFixture = mStaticBody->CreateFixture(&staticFixtureDef);
	staticFixture->SetUserData(this);

	mStaticFixtures.push_back( staticFixture );
    
    b2PolygonShape dynamicBodyShape;
    dynamicBodyShape.Set(dynamicVertices, vertexCount);
    b2FixtureDef dynamicFixtureDef;
    dynamicFixtureDef.friction = 0.0f;
    dynamicFixtureDef.shape = &dynamicBodyShape;
    dynamicFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
    dynamicFixtureDef.filter.maskBits = CollisionListener::EntityCategory::SENSOR;
    
    b2Fixture* dynamicFixture = mBody->CreateFixture(&dynamicFixtureDef);
    dynamicFixture->SetUserData(this);
    
    mDynamicFixtures.push_back( dynamicFixture );
}

// PlayEntity

Animation* Wall::newBulletStruckAnimation() {
	sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::greyBulletStrikeAnimation_png);;
	return new Animation(false, 12, image);
}

void Wall::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void Wall::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}
