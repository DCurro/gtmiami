#ifndef COLLISIONFILTER_HPP_
#define COLLISIONFILTER_HPP_

#include <Box2D/Box2D.h>


class CollisionContactFilter : public b2ContactFilter {
public:
	CollisionContactFilter();
	virtual ~CollisionContactFilter();

	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
};

#endif /* COLLISIONFILTER_HPP_ */
