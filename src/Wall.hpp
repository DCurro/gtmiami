#ifndef WALL_HPP_
#define WALL_HPP_

#include "PlayEntity.hpp"

class Animation;

class Wall : public PlayEntity {
public:
	enum WallOrientations {
		NONE=0,
		RIGHT=1,
		TOP=2,
		LEFT=3,
		BOTTOM=4,

		TOP_RIGHT = 5,
		TOP_LEFT = 6,
		BOTTOM_RIGHT = 7,
		BOTTOM_LEFT = 8,
	};

protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

    GETTER(std::vector<b2Fixture*>, StaticFixtures);
	GETTER(std::vector<b2Fixture*>, DynamicFixtures);
	GETTER(int, WallOrientation);

	GETTER(sf::Texture, WallTexture);

	Animation* mAnimation;

public:
    ACCESSOR(b2Body*, StaticBody);
    
	Wall(PlayContext* playContext, b2Body* ownerBody, b2Body* ownerStatic, int wallOrientation, b2Vec2 worldPosition, int floor);
	virtual ~Wall();
    
	virtual void update(float timeStep) override;
    virtual void draw(sf::RenderTarget& window) override;

private:
	void addFixture(b2Vec2 staticVertices[], b2Vec2 dynamicVertices[], int vertexCount);
};

#endif /* WALL_HPP_ */
