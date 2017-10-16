#include "CollisionContactFilter.hpp"

#include <Box2D/Dynamics/b2WorldCallbacks.cpp>
#include <iostream>

#include "PlayEntity.hpp"

CollisionContactFilter::CollisionContactFilter() {
}

CollisionContactFilter::~CollisionContactFilter() {
}

bool CollisionContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
	PlayEntity* playEntityA = (PlayEntity*)fixtureA->GetUserData();
	if(playEntityA == NULL) {
		 playEntityA = (PlayEntity*)fixtureA->GetBody()->GetUserData();
	}

	PlayEntity* playEntityB = (PlayEntity*)fixtureB->GetUserData();
	if(playEntityB == NULL) {
		 playEntityB = (PlayEntity*)fixtureB->GetBody()->GetUserData();
	}

	if(playEntityA != NULL && playEntityB !=NULL && playEntityA!=playEntityB) {
		if( playEntityA->isOnFloor(playEntityB->getCurrentFloor()) || playEntityB->isOnFloor(playEntityA->getCurrentFloor())) {
			return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
		}
	}

	return false;
}
