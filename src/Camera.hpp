#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "Pool.hpp"
#include "PlayEntity.hpp"

class PooledPlayEntity;

class Camera : public PlayEntity {

protected:
	//PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	PooledPlayEntity* mFocus;

	float mZoomLevel;
	sf::View mView;

public:
	static const int PIXELS_PER_METER_AT_DEFAULT_VIEW = 50;
	static constexpr float METERS_PER_PIXEL_AT_DEFAULT_VIEW = (1.0f/PIXELS_PER_METER_AT_DEFAULT_VIEW);

	Camera(sf::Vector2f location);
	virtual ~Camera();

	void setLocation(int floor, sf::Vector2f location);
	sf::Vector2f getLocation();

	virtual void update(float timeStep);

	sf::View getView();
	void reset(); //returns camera to default position and zoom
	void zoom(float zoomValue);
	void setZoomLevel(float zoomLevel);
	float getZoomLevel();
	void offsetCamera(sf::Vector2f offsetVector);
	sf::Vector2f getCameraSize();

	void requestImmediateFocus(PooledPlayEntity* focalEntity);

	static sf::Vector2f screenToWorld(sf::Vector2f screenCoords);
	sf::Vector2f screenToWorld_relative(sf::Vector2f screenCoords);
	static sf::Vector2f worldToScreen(sf::Vector2f worldCoords);
};

#endif /* CAMERA_HPP_ */
