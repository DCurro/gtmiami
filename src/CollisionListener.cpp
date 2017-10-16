#include "CollisionListener.hpp"

#include <iostream>

#include "ClassChecker.hpp"
#include "PlayEntity.hpp"
#include "Hero.hpp"
#include "Turret.hpp"
#include "Automobile.hpp"
#include "Gun.hpp"
#include "Stairwell.hpp"
#include "RocketProjectile.hpp"
#include "NavigationCell.hpp"
#include "District.hpp"
#include "Bunny.hpp"

CollisionListener::CollisionListener() {
}

CollisionListener::~CollisionListener() {
}

void CollisionListener::BeginContact(b2Contact* contact) {
	PlayEntity* dataA = static_cast<PlayEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	PlayEntity* dataB = static_cast<PlayEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (beginCheckCollisionBetweenAnimalWithCarDoors(contact, dataA, dataB))
    {
		return;
    }
    else if (beginCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(contact, dataA, dataB))
    {
		return;
	}
    else if (beginCheckCollisionBetweenBadGuyVisionSensorWithHero(contact, dataA, dataB))
    {
		return;
	}
    else if (beginCheckCollisionBetweenGunWithAnAnimal(contact, dataA, dataB))
    {
		return;
	}
    else if (beginCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(contact, dataA, dataB))
    {
		return;
	}
    else if (beginCheckCollisionBetweenRocketsWithAnything(contact, dataA, dataB))
    {
		return;
	}
    else
    {
        int x = 0;
        x++;
    }
}

void CollisionListener::EndContact(b2Contact* contact){
	UNUSED(contact);
	PlayEntity* dataA = static_cast<PlayEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	PlayEntity* dataB = static_cast<PlayEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if(endCheckCollisionBetweenAnimalWithCarDoors(contact, dataA, dataB)){
		return;
	}
	else if(endCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(contact, dataA, dataB))
    {
		return;
	}
	else if(endCheckCollisionBetweenBadGuyVisionSensorWithHero(contact, dataA, dataB))
    {
		return;
	}
	else if(endCheckCollisionBetweenGunWithAnAnimal(contact, dataA, dataB))
    {
		return;
	}
	else if(endCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(contact, dataA, dataB))
    {
		return;
	}
	else if(endCheckCollisionBetweenRocketsWithAnything(contact, dataA, dataB))
    {
		return;
	}
    else
    {
        int x = 0;
        x++;
    }
}

bool CollisionListener::beginCheckCollisionBetweenAnimalWithCarDoors(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	Animal* animal = NULL;
	Automobile* automobile = NULL;
	if( !resolveDatas(animal, automobile, dataA, dataB) ) {
		return false;
	}

	std::vector<b2Fixture*>* automobileFixtures = automobile->getPriorityOrderedDoorSensorFixtures();

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( ( fixtureA == animal->getBodyFixture() || std::find(automobileFixtures->begin(),automobileFixtures->end(),fixtureA)!=automobileFixtures->end()) ){
		if( fixtureB == animal->getBodyFixture() || std::find(automobileFixtures->begin(),automobileFixtures->end(),fixtureB)!=automobileFixtures->end() ){
			animal->getLocalVehiclesMultiSet()->insert(automobile);
			return true;
		}
	}

	return false;
}

bool CollisionListener::endCheckCollisionBetweenAnimalWithCarDoors(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	Animal* animal = NULL;
	Automobile* automobile = NULL;
	if( !resolveDatas(animal, automobile, dataA, dataB) ) {
		return false;
	}

	std::vector<b2Fixture*>* automobileFixtures = automobile->getPriorityOrderedDoorSensorFixtures();

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( (fixtureA == animal->getBodyFixture() || std::find(automobileFixtures->begin(),automobileFixtures->end(),fixtureA)!=automobileFixtures->end()) &&
		(fixtureB == animal->getBodyFixture() || std::find(automobileFixtures->begin(),automobileFixtures->end(),fixtureB)!=automobileFixtures->end()))
	{
		auto pos = animal->getLocalVehiclesMultiSet()->find(automobile);
		if (pos != animal->getLocalVehiclesMultiSet()->end()) {
			animal->getLocalVehiclesMultiSet()->erase(pos);
			return true;
		}
	}

	return false;
}

//Rockets
bool CollisionListener::beginCheckCollisionBetweenRocketsWithAnything(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	PlayEntity* anything = NULL;
	RocketProjectile* rocket = NULL;

	if( !resolveDatas(rocket, anything, dataA, dataB) ) {
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( fixtureB == rocket->getBodyFixture() ) {
		anything = (PlayEntity*)fixtureA->GetUserData();
		if(anything == NULL) {
			anything =(PlayEntity*)fixtureA->GetBody()->GetUserData();
		}

		rocket = (RocketProjectile*)fixtureB->GetUserData();
		if(rocket ==NULL) {
			rocket = (RocketProjectile*)fixtureB->GetBody()->GetUserData();
		}

		std::cout << "rocketB collides with objectA" << std::endl;

		rocket->rocketCollisionExplosion();

		std::cout << "rocket explodes successfully" << std::endl;

		return true;

	} else if( fixtureA == rocket->getBodyFixture() ) {
		anything = (PlayEntity*)fixtureB->GetUserData();
		if(anything == NULL) {
			anything =(PlayEntity*)fixtureB->GetBody()->GetUserData();
		}

		rocket = (RocketProjectile*)fixtureA->GetUserData();
		if(rocket ==NULL) {
			rocket = (RocketProjectile*)fixtureA->GetBody()->GetUserData();
		}

		std::cout << "rocketA collides with objectB" << std::endl;

		rocket->rocketCollisionExplosion();

		std::cout << "rocket explodes successfully" << std::endl;

		return true;

	} else {
		std::cout << "No collision happened" << std::endl;
	}

	return false;

}

bool CollisionListener::endCheckCollisionBetweenRocketsWithAnything(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	UNUSED(contact);

	PlayEntity* anything = NULL;
	RocketProjectile* rocket = NULL;

	if( !resolveDatas(rocket, anything, dataA, dataB) ) {
		return false;
	}
	return true;
}

bool CollisionListener::beginCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	PlayEntity* obstruction = NULL;
	Automobile* automobile = NULL;

	if( !resolveDatas(automobile, obstruction, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	for(unsigned long i=0; i<automobile->getPriorityOrderedDoorSensorFixtures()->size(); i++) {
		b2Fixture* doorFixture = automobile->getPriorityOrderedDoorSensorFixtures()->at(i);

		if((fixtureA == doorFixture || fixtureB == doorFixture) && !(fixtureA->IsSensor() && fixtureB->IsSensor())) {
			automobile->getDoorExitLocationObstructionMultiSetList()->at(i)->insert(obstruction);
			return true;
		}
	}

//	in the chance that obstruction is also a automobile, the above may fail.
	if( ClassChecker().isKindOfClass<Automobile*>(dataB) ){
		Automobile* automobile2 = dynamic_cast<Automobile*>(dataB);

		for(unsigned long i=0; i<automobile2->getPriorityOrderedDoorSensorFixtures()->size(); i++) {
			b2Fixture* doorFixture = automobile2->getPriorityOrderedDoorSensorFixtures()->at(i);

																	//only care if they are both sensors
			if((fixtureA == doorFixture || fixtureB == doorFixture) && !(fixtureA->IsSensor() && fixtureB->IsSensor())) {
				automobile2->getDoorExitLocationObstructionMultiSetList()->at(i)->insert(obstruction);
				return true;
			}
		}
	}

	return false;
}

bool CollisionListener::endCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	PlayEntity* obstruction = NULL;
	Automobile* automobile = NULL;

	if( !resolveDatas(automobile, obstruction, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	for(unsigned long i=0; i<automobile->getPriorityOrderedDoorSensorFixtures()->size(); i++) {
		b2Fixture* doorFixture = automobile->getPriorityOrderedDoorSensorFixtures()->at(i);

		if(fixtureA == doorFixture || fixtureB == doorFixture) {
			auto pos = automobile->getDoorExitLocationObstructionMultiSetList()->at(i)->find(obstruction);
			if (pos != automobile->getDoorExitLocationObstructionMultiSetList()->at(i)->end()) {
				automobile->getDoorExitLocationObstructionMultiSetList()->at(i)->erase(pos);
				return true;
			}
		}
	}

	//in the chance that obstruction is also a automobile, the above may fail.
	if( ClassChecker().isKindOfClass<Automobile*>(dataB) ){
		Automobile* automobile2 = dynamic_cast<Automobile*>(dataB);

		for(unsigned long i=0; i<automobile2->getPriorityOrderedDoorSensorFixtures()->size(); i++) {
			b2Fixture* doorFixture = automobile2->getPriorityOrderedDoorSensorFixtures()->at(i);

			if(fixtureA == doorFixture || fixtureB == doorFixture) {
				auto pos = automobile2->getDoorExitLocationObstructionMultiSetList()->at(i)->find(obstruction);
				if (pos != automobile2->getDoorExitLocationObstructionMultiSetList()->at(i)->end()) {
					automobile2->getDoorExitLocationObstructionMultiSetList()->at(i)->erase(pos);
					return true;
				}
			}
		}
	}

	return false;
}

bool CollisionListener::beginCheckCollisionBetweenBadGuyVisionSensorWithHero(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB){
	Turret* turret = NULL;
	Hero* hero = NULL;
	if( !resolveDatas(hero, turret, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( (fixtureA == turret->getVisionSensorFixture() || fixtureB == turret->getVisionSensorFixture()) &&
		(fixtureA == hero->getBodyFixture() || fixtureB == hero->getBodyFixture()) ){
		turret->getVisionSet()->insert(hero);
		turret->setTargetEntity(hero);

		return true;
	}

	return false;
}

bool CollisionListener::endCheckCollisionBetweenBadGuyVisionSensorWithHero(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB){
	Turret* badGuy = NULL;
	Hero* hero = NULL;
	if( !resolveDatas(hero, badGuy, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( (fixtureA == badGuy->getVisionSensorFixture() || fixtureB == badGuy->getVisionSensorFixture()) &&
		(fixtureA == hero->getBodyFixture() || fixtureB == hero->getBodyFixture()) ){
		if(badGuy->getVisionSet()->find(hero) != badGuy->getVisionSet()->end()){
			badGuy->getVisionSet()->erase(hero);
			badGuy->setTargetEntity(NULL);
		}

		return true;
	}

	return false;
}

bool CollisionListener::beginCheckCollisionBetweenGunWithAnAnimal(b2Contact* contact,PlayEntity* dataA,PlayEntity* dataB) {
	Animal* animal = NULL;
	Weapon* weapon = NULL;
	if( !resolveDatas(animal, weapon, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( (fixtureA == animal->getBodyFixture() || fixtureB == animal->getBodyFixture()) &&
		(fixtureA == weapon->getBodyFixture() || fixtureB == weapon->getBodyFixture()) ){
		animal->getWeaponsInPickupRange()->push_back(weapon);

		return true;
	}

	return false;
}

bool CollisionListener::endCheckCollisionBetweenGunWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	Animal* animal = NULL;
	Weapon* weapon = NULL;
	if( !resolveDatas(animal, weapon, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if( (fixtureA == animal->getBodyFixture() || fixtureB == animal->getBodyFixture()) &&
		(fixtureA == weapon->getBodyFixture() || fixtureB == weapon->getBodyFixture()) ){
		auto gunList = animal->getWeaponsInPickupRange();
		gunList->erase(std::remove(gunList->begin(), gunList->end(), weapon), gunList->end());

		return true;
	}

	return false;
}

bool CollisionListener::beginCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(b2Contact* contact,PlayEntity* dataA,PlayEntity* dataB) {
	Animal* animal = NULL;
	Stairwell* stairwell = NULL;
	if( !resolveDatas(animal, stairwell, dataA, dataB) ){
		return false;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//going up
	if( (fixtureA == stairwell->getUpStairsFixtureSensor() || fixtureB == stairwell->getUpStairsFixtureSensor()) &&
		(fixtureA == animal->getBodyFixture() || fixtureB == animal->getBodyFixture()) ){
		std::cout << "up stairs" << std::endl;
		animal->setCurrentFloor(1.0f);
		return true;
	}

	//going down
	if( (fixtureA == stairwell->getDownStairsFixtureSensor() || fixtureB == stairwell->getDownStairsFixtureSensor()) &&
		(fixtureA == animal->getBodyFixture() || fixtureB == animal->getBodyFixture()) ){
		std::cout << "down stairs" << std::endl;
		animal->setCurrentFloor(0.0f);
		return true;
	}

	return false;
}

bool CollisionListener::endCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB) {
	UNUSED(contact);

	Animal* animal = NULL;
	Stairwell* stairwell = NULL;
	if( !resolveDatas(animal, stairwell, dataA, dataB) ){
		return false;
	}

	return true; //nothing to do on end
}

// Private Helpers

template<typename TypeA, typename TypeB>
bool CollisionListener::resolveDatas(TypeA &resolutionA, TypeB &resolutionB, PlayEntity* dataA, PlayEntity* dataB) {
	//dataA is of both types, dataB is specific
	if( ClassChecker().isKindOfClass<TypeA>(dataA) && ClassChecker().isKindOfClass<TypeB>(dataA) ){
		if( ClassChecker().isKindOfClass<TypeA>(dataB)){
			resolutionA = dynamic_cast<TypeA>(dataB);
			resolutionB = dynamic_cast<TypeB>(dataA);
			return true;
		} else if(  ClassChecker().isKindOfClass<TypeB>(dataB) ) {
			resolutionB = dynamic_cast<TypeB>(dataB);
			resolutionA = dynamic_cast<TypeA>(dataA);
			return true;
		}
	}

	//dataB is of both types, dataA is specfic
	if( ClassChecker().isKindOfClass<TypeA>(dataB) && ClassChecker().isKindOfClass<TypeB>(dataB) ){
		if( ClassChecker().isKindOfClass<TypeA>(dataA)){
			resolutionA = dynamic_cast<TypeA>(dataA);
			resolutionB = dynamic_cast<TypeB>(dataB);
			return true;
		} else if(  ClassChecker().isKindOfClass<TypeB>(dataA) ) {
			resolutionB = dynamic_cast<TypeB>(dataA);
			resolutionA = dynamic_cast<TypeA>(dataB);
			return true;
		}
	}

	//they both datas are of both types
	if( ClassChecker().isKindOfClass<TypeA>(dataA) && ClassChecker().isKindOfClass<TypeB>(dataA) &&
		ClassChecker().isKindOfClass<TypeA>(dataB) && ClassChecker().isKindOfClass<TypeB>(dataB) )
	{
		resolutionA = dynamic_cast<TypeA>(dataA);
		resolutionB = dynamic_cast<TypeB>(dataB);
	}

	//the types are unique
	if( ClassChecker().isKindOfClass<TypeA>(dataA) ){
		resolutionA = dynamic_cast<TypeA>(dataA);
		if( ClassChecker().isKindOfClass<TypeB>(dataB) ){
			resolutionB = dynamic_cast<TypeB>(dataB);
			return true;
		}
	}

	if( ClassChecker().isKindOfClass<TypeA>(dataB) ){
		resolutionA = dynamic_cast<TypeA>(dataB);
		if( ClassChecker().isKindOfClass<TypeB>(dataA) ){
			resolutionB = dynamic_cast<TypeB>(dataA);
			return true;
		}
	}

	return false;
}
