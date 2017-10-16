#include "Factory.hpp"

#include <iostream>

#include "CollisionListener.hpp"
#include "common.hpp"
#include "PlayContext.hpp"
#include "ResourceManager.hpp"
#include "GameContext.hpp"

#include "Mind.hpp"
#include "HeroMind.hpp"
#include "TurretMind.hpp"
#include "BunnyMind.hpp"
#include "ZombieMind.hpp"

#include "PooledPlayEntityDescriptor.hpp"
#include "BunnyDescriptor.hpp"
#include "TurretDescriptor.hpp"
#include "ZombieDescriptor.hpp"
#include "SpiritDescriptor.hpp"
#include "CarDescriptor.hpp"
#include "TowTruckDescriptor.hpp"
#include "RevolverDescriptor.hpp"
#include "Ak47Descriptor.hpp"
#include "RocketLauncherDescriptor.hpp"
#include "GrenadeDescriptor.hpp"

#include "RandomIdentifierProvider.hpp"

#define HUMAN_BODY_RADIUS 0.25f
#define DOOR_SENSOR_RADIUS 0.25f

Factory::Factory(){
}

Factory::~Factory() {
}

PooledPlayEntity* Factory::pooledPlayEntityForDescriptor(PlayContext* playContext,
                                                         PooledPlayEntityDescriptor *pooledPlayEntityDescriptor)
{
    if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_BUNNY) == 0) {
        return this->createBunnyFromDescriptor(playContext, static_cast<BunnyDescriptor*>(pooledPlayEntityDescriptor));
    
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_TURRET) == 0) {
        return this->createTurretFromDescriptor(playContext, static_cast<TurretDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_ZOMBIE) == 0) {
        return this->createZombieFromDescriptor(playContext, static_cast<ZombieDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_CAR) == 0) {
        return this->createCarFromDescriptor(playContext, static_cast<CarDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_TOWTRUCK) == 0) {
        return this->createTowTruckFromDescriptor(playContext, static_cast<TowTruckDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_REVOLVER) == 0) {
        return this->createRevolverFromDescriptor(playContext, static_cast<RevolverDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_AK47) == 0) {
        return this->createAk47FromDescriptor(playContext, static_cast<Ak47Descriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_ROCKETLAUNCHER) == 0) {
        return this->createRocketLauncherFromDescriptor(playContext, static_cast<RocketLauncherDescriptor*>(pooledPlayEntityDescriptor));
        
    } else if (pooledPlayEntityDescriptor->getPOOLEDPLAYENTITY_TYPE().compare(POOLEDPLAYENTITY_TYPE_GRENADE) == 0) {
        return this->createGrenadeFromDescriptor(playContext, static_cast<GrenadeDescriptor*>(pooledPlayEntityDescriptor));
        
    } else {
        assert(false);
    }
}

#pragma mark - Private Dynamic approach support

Bunny* Factory::createBunnyFromDescriptor(PlayContext* playContext, BunnyDescriptor *descriptor) {
    Bunny* entity = this->createBunny(playContext,
                                      descriptor->getUniqueReferenceId(),
                                      descriptor->getFloor(),
                                      b2Vec2(descriptor->getX(), descriptor->getY()),
                                      descriptor->getAngle());
    return entity;
}

Turret* Factory::createTurretFromDescriptor(PlayContext* playContext, TurretDescriptor* descriptor) {
    Turret* entity = this->createTurret(playContext,
                                        descriptor->getUniqueReferenceId(),
                                        descriptor->getFloor(),
                                        b2Vec2(descriptor->getX(), descriptor->getY()),
                                        descriptor->getAngle());
    return entity;
}

Zombie* Factory::createZombieFromDescriptor(PlayContext *playContext, ZombieDescriptor* descriptor) {
    Zombie* entity = this->createZombie(playContext,
                                        descriptor->getUniqueReferenceId(),
                                        descriptor->getFloor(),
                                        b2Vec2(descriptor->getX(), descriptor->getY()),
                                        descriptor->getAngle());
    return entity;
}


Car* Factory::createCarFromDescriptor(PlayContext* playContext, CarDescriptor *descriptor) {
    Car* entity = this->createCar(playContext,
                                  descriptor->getUniqueReferenceId(),
                                  descriptor->getFloor(),
                                  b2Vec2(descriptor->getX(), descriptor->getY()),
                                  descriptor->getAngle());
    return entity;
}

TowTruck* Factory::createTowTruckFromDescriptor(PlayContext* playContext, TowTruckDescriptor *descriptor) {
    TowTruck* entity = this->createTowTruck(playContext,
                                            descriptor->getUniqueReferenceId(),
                                            descriptor->getFloor(),
                                            b2Vec2(descriptor->getX(), descriptor->getY()),
                                            descriptor->getAngle());
    return entity;
}


Revolver* Factory::createRevolverFromDescriptor(PlayContext* playContext, RevolverDescriptor *descriptor) {
    Revolver* entity = this->createRevolver(playContext,
                                            descriptor->getUniqueReferenceId(),
                                            descriptor->getFloor(),
                                            b2Vec2(descriptor->getX(), descriptor->getY()),
                                            descriptor->getAngle());
    return entity;
}

Ak47* Factory::createAk47FromDescriptor(PlayContext* playContext, Ak47Descriptor *descriptor) {
    Ak47* entity = this->createAk47(playContext,
                                    descriptor->getUniqueReferenceId(),
                                    descriptor->getFloor(),
                                    b2Vec2(descriptor->getX(), descriptor->getY()),
                                    descriptor->getAngle());
    return entity;
}

RocketLauncher* Factory::createRocketLauncherFromDescriptor(PlayContext* playContext, RocketLauncherDescriptor *descriptor) {
    RocketLauncher* entity = this->createRocketLauncher(playContext,
                                                        descriptor->getUniqueReferenceId(),
                                                        descriptor->getFloor(),
                                                        b2Vec2(descriptor->getX(), descriptor->getY()),
                                                        descriptor->getAngle());
    return entity;
}

Grenade* Factory::createGrenadeFromDescriptor(PlayContext* playContext, GrenadeDescriptor *descriptor) {
    Grenade* entity = this->createGrenade(playContext,
                                          descriptor->getUniqueReferenceId(),
                                          descriptor->getFloor(),
                                          b2Vec2(descriptor->getX(), descriptor->getY()),
                                          descriptor->getAngle());
    return entity;
}

#pragma mark - creators

Hero* Factory::createHero(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angleInDegrees){
	Hero* entity = mHeroPool.take();

	HeroMind* mind = new HeroMind();
	mind->setup(playContext, entity);

	entity->setup(playContext, 0, mind, floor); // TODO: make a constant NO_IDENTIFIER_PROVIDED

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2World* world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape bodyShape;
	bodyShape.m_radius = HUMAN_BODY_RADIUS;
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 500.0f;   //density*area = 180kg Hero
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
    bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING - CollisionListener::EntityCategory::CAR;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);

	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);
    
	entity->setBody(body);
    
	return entity;
}

Turret* Factory::createTurret(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees){
	Turret* entity = mTurretPool.take();

	TurretMind* mind = new TurretMind();
	mind->setup(playContext, entity);

	entity->setup(playContext, uniqueReferenceId, mind, floor);
	entity->setMind(mind);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2World* world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape bodyShape;
	bodyShape.m_radius = HUMAN_BODY_RADIUS;
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 500.0f;
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2PolygonShape visionShape;
	b2Vec2 vertices[] = { b2Vec2(0,0), b2Vec2(6,-2), b2Vec2(6,2) };
	visionShape.Set(vertices, 3); //pass array to the shape
	b2FixtureDef visionSensorFixtureDef;
    visionSensorFixtureDef.friction = 0.0f;
	visionSensorFixtureDef.shape = &visionShape;
	visionSensorFixtureDef.isSensor = true;
	visionSensorFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	visionSensorFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);
	b2Fixture* visionSensorFixture = body->CreateFixture(&visionSensorFixtureDef);

	entity->setBodyFixture(bodyFixture);
	entity->setVisionSensorFixture(visionSensorFixture);

	body->SetUserData(entity);

	entity->setBody(body);

	return entity;
}

Bunny* Factory::createBunny(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees){
	Bunny* entity = mBunnyPool.take();
    
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2World* world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape bodyShape;
    bodyShape.m_radius = HUMAN_BODY_RADIUS/2.0f;//*2.4f;
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 500.0f;   //density*area = 180kg Hero
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING - CollisionListener::EntityCategory::CAR;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);

    BunnyMind * mind = new BunnyMind();
    entity->setup(playContext, uniqueReferenceId, mind, floor);
    
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	entity->setBody(body);
    mind->setup(playContext, entity);
    
	return entity;
}

Zombie * Factory::createZombie(PlayContext *playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees) {
    Zombie * entity = mZombiePool.take();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(worldPosition.x, worldPosition.y);
    bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
    b2World* world = playContext->getWorld();
    b2Body* body = world->CreateBody(&bodyDef);
    
    b2CircleShape bodyShape;
    bodyShape.m_radius = HUMAN_BODY_RADIUS;
    b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
    bodyFixtureDef.shape = &bodyShape;
    bodyFixtureDef.density = 500.0f;   //density*area = 180kg Hero
    bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
    bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING - CollisionListener::EntityCategory::CAR;
    
    b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);
    
    ZombieMind * mind = new ZombieMind();
    entity->setup(playContext, uniqueReferenceId, mind, floor);
    
    entity->setBodyFixture(bodyFixture);
    
    body->SetUserData(entity);
    
    entity->setBody(body);
    mind->setup(playContext, entity);
    
    return entity;
}

Spirit* Factory::createSpirit(PlayContext* playContext, int floor, Mind* mind, b2Vec2 worldPosition, float angleInDegrees){
	Spirit* entity = mSpiritPool.take();

	mind->setup(playContext, entity);

	entity->setup(playContext, 0, mind, floor); // TODO: make a constant for 0 NO_PERSISTENCE_IDENTIFIER

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2World* world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape bodyShape;
	bodyShape.m_radius = HUMAN_BODY_RADIUS/2.0f;
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 500.0f;   //density*area = 180kg Hero
	bodyFixtureDef.isSensor = true;
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);

	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);


	entity->setBody(body);

	return entity;
}

Car* Factory::createCar(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees){
	Car* entity = mCarPool.take();

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	bodyDef.linearDamping = 0.3f;
	world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	entity->setup(playContext, uniqueReferenceId, floor, body, 500);

	b2BodyDef staticBodyDef;
	staticBodyDef.type = b2_staticBody;
	staticBodyDef.position.Set(worldPosition.x, worldPosition.y);
	staticBodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	world = playContext->getWorld();
	b2Body* staticBody = world->CreateBody(&staticBodyDef);

	b2PolygonShape bodyShape;
	bodyShape.SetAsBox(2.05f, 0.9f);
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 137.25f;
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::CAR;
	bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2PolygonShape staticBodyShape;
	staticBodyShape.SetAsBox(2.0f, 0.85f);
	b2FixtureDef staticBodyFixtureDef;
    staticBodyFixtureDef.friction = 0.0f;
	staticBodyFixtureDef.shape = &staticBodyShape;
	staticBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::CLASS_3_WEIGHTED_OBJECT;
    staticBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT | CollisionListener::EntityCategory::SENSOR;

	//for determining if it's safe to exit vehicle
	b2CircleShape driverExitShape;
	driverExitShape.m_radius = DOOR_SENSOR_RADIUS;
	driverExitShape.m_p = b2Vec2(0.0f,-1.2f);
	b2FixtureDef driverExitFixtureDef;
    driverExitFixtureDef.friction = 0.0f;
	driverExitFixtureDef.shape = &driverExitShape;
	driverExitFixtureDef.isSensor = true;
	driverExitFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	driverExitFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2CircleShape passengerExitShape;
	passengerExitShape.m_radius = DOOR_SENSOR_RADIUS;
	passengerExitShape.m_p = b2Vec2(0.0f, 1.2f);
	b2FixtureDef passengerExitFixtureDef;
    passengerExitFixtureDef.friction = 0.0f;
	passengerExitFixtureDef.shape = &passengerExitShape;
	passengerExitFixtureDef.isSensor = true;
	passengerExitFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	passengerExitFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);
	b2Fixture* driverDoorExitFixture = body->CreateFixture(&driverExitFixtureDef);
	b2Fixture* passengerDoorExitFixture = body->CreateFixture(&passengerExitFixtureDef);

	b2Fixture* bodyStaticFixture = staticBody->CreateFixture(&staticBodyFixtureDef);
    staticBody->SetSleepingAllowed(false);

	entity->setBodyFixture(bodyFixture);
	entity->addDoorSensorFixtureAsNextMostPriority(driverDoorExitFixture);
	entity->addDoorSensorFixtureAsNextMostPriority(passengerDoorExitFixture);

	body->SetUserData(entity);
	staticBody->SetUserData(entity);

	entity->setStaticBody(staticBody);
    entity->setStaticBodyFixture(bodyStaticFixture);

	return entity;
}

TowTruck* Factory::createTowTruck(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees){
	TowTruck* entity = mTowTruckPool.take();

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	bodyDef.linearDamping = 0.3f;
	world = playContext->getWorld();
	b2Body* body = world->CreateBody(&bodyDef);

	entity->setup(playContext, uniqueReferenceId, floor, body, 500);

	b2BodyDef staticBodyDef;
	staticBodyDef.type = b2_staticBody;
	staticBodyDef.position.Set(worldPosition.x, worldPosition.y);
	staticBodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	world = playContext->getWorld();
	b2Body* staticBody = world->CreateBody(&staticBodyDef);

	b2PolygonShape bodyShape;
	bodyShape.SetAsBox(2.925f, 1.2f);
	b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.friction = 0.0f;
	bodyFixtureDef.shape = &bodyShape;
	bodyFixtureDef.density = 137.25f;
	bodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::CAR;
	bodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2PolygonShape staticBodyShape;
	staticBodyShape.SetAsBox(2.875f, 1.15f);
	b2FixtureDef staticBodyFixtureDef;
    staticBodyFixtureDef.friction = 0.0f;
	staticBodyFixtureDef.shape = &staticBodyShape;
	staticBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::CLASS_3_WEIGHTED_OBJECT;
	staticBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2CircleShape driverExitShape;
	driverExitShape.m_radius = DOOR_SENSOR_RADIUS;
	driverExitShape.m_p = b2Vec2(1.0f,-1.35f);
	b2FixtureDef driverExitFixtureDef;
    driverExitFixtureDef.friction = 0.0f;
	driverExitFixtureDef.shape = &driverExitShape;
	driverExitFixtureDef.isSensor = true;
	driverExitFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	driverExitFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2CircleShape passengerExitShape;
	passengerExitShape.m_radius = DOOR_SENSOR_RADIUS;
	passengerExitShape.m_p = b2Vec2(1.0f, 1.35f);
	b2FixtureDef passengerExitFixtureDef;
    passengerExitFixtureDef.friction = 0.0f;
	passengerExitFixtureDef.shape = &passengerExitShape;
	passengerExitFixtureDef.isSensor = true;
	passengerExitFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	passengerExitFixtureDef.filter.maskBits = CollisionListener::EntityCategory::EVERYTHING;

	b2Fixture* bodyFixture = body->CreateFixture(&bodyFixtureDef);
	b2Fixture* driverDoorExitFixture = body->CreateFixture(&driverExitFixtureDef);
	b2Fixture* passengerDoorExitFixture = body->CreateFixture(&passengerExitFixtureDef);

	b2Fixture* bodyStaticFixture = staticBody->CreateFixture(&staticBodyFixtureDef);
    staticBody->SetSleepingAllowed(false);

	entity->setBodyFixture(bodyFixture);
	entity->addDoorSensorFixtureAsNextMostPriority(driverDoorExitFixture);
	entity->addDoorSensorFixtureAsNextMostPriority(passengerDoorExitFixture);

	body->SetUserData(entity);
	staticBody->SetUserData(entity);

	entity->setStaticBody(staticBody);
    entity->setStaticBodyFixture(bodyStaticFixture);

	return entity;
}

Revolver* Factory::createRevolver(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	Revolver* entity = mRevolverPool.take();
	entity->setup(playContext, uniqueReferenceId, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.25f;
	b2FixtureDef gunBodyFixtureDef;
    gunBodyFixtureDef.friction = 0.0f;
	gunBodyFixtureDef.shape = &circleGunShape;
	gunBodyFixtureDef.isSensor = true;
	gunBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	gunBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&gunBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

Ak47* Factory::createAk47(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	Ak47* entity = mAk47Pool.take();
	entity->setup(playContext, uniqueReferenceId, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.25f;
	b2FixtureDef gunBodyFixtureDef;
    gunBodyFixtureDef.friction = 0.0f;
	gunBodyFixtureDef.shape = &circleGunShape;
	gunBodyFixtureDef.isSensor = true;
	gunBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	gunBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&gunBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

RocketLauncher* Factory::createRocketLauncher(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	RocketLauncher* entity = mRocketLauncherPool.take();
	entity->setup(playContext, uniqueReferenceId, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.25f;
	b2FixtureDef gunBodyFixtureDef;
    gunBodyFixtureDef.friction = 0.0f;
	gunBodyFixtureDef.shape = &circleGunShape;
	gunBodyFixtureDef.isSensor = true;
	gunBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	gunBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&gunBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

RocketProjectile* Factory::createRocketProjectile(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	RocketProjectile* entity = mRocketProjectilePool.take();

	entity->setup(playContext, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	bodyDef.linearDamping = 0.f;
	bodyDef.angularDamping = 0.f;
	b2Body* body = world->CreateBody(&bodyDef);


	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.125f;
	b2FixtureDef rocketBodyFixtureDef;
    rocketBodyFixtureDef.friction = 0.0f;
	rocketBodyFixtureDef.shape = &circleGunShape;
	rocketBodyFixtureDef.density = 0.625f;
	rocketBodyFixtureDef.restitution = 0.5f;
	rocketBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	rocketBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&rocketBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

Grenade* Factory::createGrenade(PlayContext* playContext, int uniqueReferenceId, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	Grenade* entity = mGrenadePool.take();
	entity->setup(playContext, uniqueReferenceId, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.25f;
	b2FixtureDef grenadeBodyFixtureDef;
    grenadeBodyFixtureDef.friction = 0.0f;
	grenadeBodyFixtureDef.shape = &circleGunShape;
	grenadeBodyFixtureDef.isSensor = true;
	grenadeBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	grenadeBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&grenadeBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

GrenadeProjectile* Factory::createGrenadeProjectile(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angleInDegrees) {
	GrenadeProjectile* entity = mGrenadeProjectilePool.take();

	entity->setup(playContext, floor);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	bodyDef.linearDamping = 0.75f;
	bodyDef.angularDamping = 0.75f;
	b2Body* body = world->CreateBody(&bodyDef);


	b2CircleShape circleGunShape;
	circleGunShape.m_radius = 0.125f;
	b2FixtureDef grenadeBodyFixtureDef;
    grenadeBodyFixtureDef.friction = 0.0f;
	grenadeBodyFixtureDef.shape = &circleGunShape;
	grenadeBodyFixtureDef.density = 0.625f;
	grenadeBodyFixtureDef.restitution = 0.5f;
	grenadeBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;
	grenadeBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&grenadeBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}

Explosion* Factory::createExplosion(PlayContext* playContext, int floor, b2Vec2 worldPosition, float angleInDegrees, float blastRadius, float baseDamage) {
	Explosion* entity = mExplosionPool.take();

	entity->setup(playContext, floor, blastRadius, baseDamage);

	b2World* world = playContext->getWorld();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(worldPosition.x, worldPosition.y);
	bodyDef.angle = angleInDegrees*DEGREES_TO_RADIANS;
	b2Body* body = world->CreateBody(&bodyDef);


	b2CircleShape circleGunShape;
	circleGunShape.m_radius = blastRadius;
	b2FixtureDef grenadeBodyFixtureDef;
    grenadeBodyFixtureDef.friction = 0.0f;
	grenadeBodyFixtureDef.shape = &circleGunShape;
	grenadeBodyFixtureDef.isSensor = true;
	grenadeBodyFixtureDef.filter.categoryBits = CollisionListener::EntityCategory::SENSOR;
	grenadeBodyFixtureDef.filter.maskBits = CollisionListener::EntityCategory::PHYSICAL_OBJECT;

	b2Fixture* bodyFixture = body->CreateFixture(&grenadeBodyFixtureDef);

	entity->setBody(body);
	entity->setBodyFixture(bodyFixture);

	body->SetUserData(entity);

	return entity;
}
