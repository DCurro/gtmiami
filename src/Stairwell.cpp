#include "Stairwell.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "Animation.hpp"
#include "CollisionListener.hpp"

Stairwell::Stairwell(PlayContext* playContext, b2Vec2 worldPosition, int startFloor) {
	mPlayContext = playContext;

	this->generateBodyAndFixtures(worldPosition);

	mCurrentFloor = startFloor;
	mNextFloor = startFloor+1;

	sf::Image* animationImage = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::stairwell_png);
	mAnimation = new Animation(false, 0, animationImage);

	mSprite.setTexture(*mAnimation->getCurrentFrame(), true);
	sf::Vector2f screenPosition = Camera::worldToScreen(sf::Vector2f(worldPosition.x,worldPosition.y));
	mSprite.setPosition(screenPosition);
}

void Stairwell::generateBodyAndFixtures(b2Vec2 worldPosition) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	b2World* world = mPlayContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	mBody = body;
	mBody->SetUserData(this);

	//railings
	b2Vec2 vertices[4];

	vertices[0].Set(-1.0f,  -1.0f);
	vertices[1].Set(-0.9f, -1.0f);
	vertices[2].Set(-0.9f, 1.0f);
	vertices[3].Set(-1.0f,  1.0f);
	this->createRailingFixture(vertices, 4);

	vertices[0].Set(1.0f,  -1.0f);
	vertices[1].Set(0.9f, -1.0f);
	vertices[2].Set(0.9f, 1.0f);
	vertices[3].Set(1.0f,  1.0f);
	this->createRailingFixture(vertices, 4);

	//sensors for going up and down

	vertices[0].Set(-0.9f,  -0.8f);
	vertices[1].Set(0.9f, -0.8f);
	vertices[2].Set(0.9f, -0.7f);
	vertices[3].Set(-0.9f,  -0.7f);
	mUpStairsFixtureSensor =  this->createFloorChangingSensorFixture(vertices, 4);

	vertices[0].Set(-0.9f,  0.7f);
	vertices[1].Set(0.9f, 0.7f);
	vertices[2].Set(0.9f, 0.8f);
	vertices[3].Set(-0.9f,  0.8f);
	mDownStairsFixtureSensor =  this->createFloorChangingSensorFixture(vertices, 4);

}

Stairwell::~Stairwell() {
	if(mBody) {
		b2World* world = mPlayContext->getWorld();
		world->DestroyBody(mBody);
		mBody = NULL;
	}
}

b2Fixture* Stairwell::createFloorChangingSensorFixture(b2Vec2 vertices[], int vertexCount) {
	b2PolygonShape sensorShape;
	sensorShape.Set(vertices, vertexCount);
	b2FixtureDef sensorFixtureDef;
    sensorFixtureDef.friction = 0.0f;
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;
	sensorFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	sensorFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	return mBody->CreateFixture(&sensorFixtureDef);
}

b2Fixture* Stairwell::createRailingFixture(b2Vec2 vertices[], int vertexCount) {
	b2PolygonShape railingShape;
	railingShape.Set(vertices, vertexCount);
	b2FixtureDef railingFixtureDef;
    railingFixtureDef.friction = 0.0f;
	railingFixtureDef.shape = &railingShape;
	railingFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	railingFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	return mBody->CreateFixture(&railingFixtureDef);
}

// PlayEntity

Animation* Stairwell::newBulletStruckAnimation() {
	return NULL;
}

void Stairwell::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void Stairwell::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}

bool Stairwell::isOnFloor(int floor) {
	return (mCurrentFloor == floor || mNextFloor == floor);
}
