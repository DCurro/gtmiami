#include "Box2DWorld.hpp"

#include <vector>
#include <algorithm>
#include "ClassChecker.hpp"
#include "NavigationCell.hpp"
#include "PlayEntity.hpp"

Box2DWorld::Box2DWorld(const b2Vec2& gravity) : b2World(gravity) {

}

Box2DWorld::~Box2DWorld() {
    
}

#pragma Protected Methods

void Box2DWorld::DrawJoint(b2Joint* joint) {
    b2World::DrawJoint(joint);
}

void Box2DWorld::DrawShape(b2Fixture* shape, const b2Transform& xf, const b2Color& color)
{
    PlayEntity* bodyUserData = NULL;
    if (shape->GetBody()->GetUserData() != NULL) {
        bodyUserData = (PlayEntity*)shape->GetBody()->GetUserData();
    }
    
    PlayEntity* fixtureUserData = NULL;
    if (shape->GetUserData() != NULL) {
        fixtureUserData = (PlayEntity*)shape->GetUserData();
    }
    
    b2World::DrawShape(shape, xf, color);
}