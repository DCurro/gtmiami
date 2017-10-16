#ifndef Box2DWorld_HPP_
#define Box2DWorld_HPP_

#include "GameObject.hpp"
#include "Common.hpp"
#include <unordered_set>

class Box2DWorld : public b2World {
public:
    Box2DWorld(const b2Vec2& gravity);
    virtual ~Box2DWorld();
    
protected:
    virtual void DrawJoint(b2Joint* joint) override;
    virtual void DrawShape(b2Fixture* shape, const b2Transform& xf, const b2Color& color) override;
};

#endif
