#ifndef STAIRWELL_HPP_
#define STAIRWELL_HPP_

#include "PlayEntity.hpp"

class Stairwell: public PlayEntity {
protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	Animation* mAnimation;

	GETTER(std::vector<b2Fixture*>, Fixtures);
	GETTER_AND_DEFAULT_NULL(b2Fixture*, UpStairsFixtureSensor);
	GETTER_AND_DEFAULT_NULL(b2Fixture*, DownStairsFixtureSensor);
	GETTER(int, NextFloor);

	void generateBodyAndFixtures(b2Vec2 worldPosition);

public:
	Stairwell(PlayContext* playContext, b2Vec2 worldPosition, int startFloor);
	virtual ~Stairwell();

	bool isOnFloor(int floor);

private:
	b2Fixture* createFloorChangingSensorFixture(b2Vec2 vertices[], int vertexCount);
	b2Fixture* createRailingFixture(b2Vec2 vertices[], int vertexCount);
};

#endif /* STAIRWELL_HPP_ */
