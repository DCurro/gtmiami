#ifndef COLLISIONLISTENER_HPP_
#define COLLISIONLISTENER_HPP_

#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include "common.hpp"

class PlayEntity;

class CollisionListener: public b2ContactListener {
public:
	enum EntityCategory {
		NO_INTERACTION   = 0x0000,
		DEFAULT          = 0x0001,
		PHYSICAL_OBJECT  = 0x0002,
		SENSOR           = 0x0004,
		CLASS_3_WEIGHTED_OBJECT = 0x0008, // TODO: rename to super heavy or something more intuitive
		CLASS_2_WEIGHTED_OBJECT = 0x0010,
		CLASS_1_WEIGHTED_OBJECT = 0x0020,
		WEAPON		     = 0x0040,
		CAR              = 0x0080,
		CATEGORY_9       = 0x0100,
		CATEGORY_10      = 0x0200,
		CATEGORY_11      = 0x0410,
		CATEGORY_12      = 0x0800,
		CATEGORY_13      = 0x1000,
		CATEGORY_14      = 0x2000,
		CATEGORY_15      = 0x4000,
		CATEGORY_16      = 0x8000,
        
		EVERYTHING       = 0xFFFF,
	};

	CollisionListener();
	virtual ~CollisionListener();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	bool beginCheckCollisionBetweenAnimalWithCarDoors(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenAnimalWithCarDoors(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);

	bool beginCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenCarExitSensorsWithPhysicalObstructions(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);

	bool beginCheckCollisionBetweenBadGuyVisionSensorWithHero(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenBadGuyVisionSensorWithHero(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);

	bool beginCheckCollisionBetweenGunWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenGunWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);

	bool beginCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenStairwellUpOrDownWithAnAnimal(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);

	bool beginCheckCollisionBetweenRocketsWithAnything(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);
	bool endCheckCollisionBetweenRocketsWithAnything(b2Contact* contact, PlayEntity* dataA, PlayEntity* dataB);    
    
	// Helpers

	template<typename TypeA, typename TypeB>
	bool resolveDatas(TypeA &resolutionA, TypeB &resolutionB, PlayEntity* dataA, PlayEntity* dataB);
};

#endif /* COLLISIONLISTENER_HPP_ */
