#include "BulletGun.hpp"

#include "Camera.hpp"
#include "BulletRayCaster.hpp"

BulletGun::BulletGun() {
}

BulletGun::~BulletGun() {
}

void BulletGun::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
	Gun::setup(playContext, persistentIdNumber, floor);
}

void BulletGun::strikeWithAngle(float angle) {
	float shotDistance = this->rangeInMeters();
	BulletRayCaster bulletCast;
	bulletCast.castRay(mPlayContext, this->getCurrentFloor(), this->getBody()->GetPosition(), angle, shotDistance);

	PlayEntity* entityStruckByBullet = bulletCast.getPlayEntityStruct();

	if(entityStruckByBullet != NULL) {
		b2Vec2 location = bulletCast.getPointOfIntersection();
		b2Vec2 normal = bulletCast.getNormal();

		float angle = std::atan2(normal.y,normal.x) * RADIANS_TO_DEGREES;

		entityStruckByBullet->strikeWithBulletAtWorldLocationWithNormalAngle(location, angle, this->bulletCalibre());
	}

	sf::Vector2f lineVector( Camera::worldToScreen(sf::Vector2f(shotDistance*bulletCast.getFraction(), 0.0f)) );

	sf::RectangleShape line(sf::Vector2f(lineVector.x, 1.0f));
	line.setPosition( Camera::worldToScreen( sf::Vector2f(this->getBody()->GetPosition().x, this->getBody()->GetPosition().y) ) );
	line.setFillColor(sf::Color(255, 255, 255, 127));
	line.rotate(angle);

	bulletCasts.push_back(line);
}
