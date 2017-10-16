#include "Projectile.hpp"

#include "Animation.hpp"

Projectile::Projectile() {
}

Projectile::~Projectile() {
}

void Projectile::update(float timeStep) {
	PooledPlayEntity::update(timeStep);

	if(this->idleAnimation()) {
		mSprite.setTexture(*this->idleAnimation()->getCurrentFrame(), true);
		this->idleAnimation()->update(timeStep);

		if(mBody!=NULL){
			mSprite.setOrigin(sf::Vector2f(mSprite.getTextureRect().width/2, mSprite.getTextureRect().height/2));

			mScreenPosition.x = worldToScreen(mBody->GetPosition().x);
			mScreenPosition.y = worldToScreen(mBody->GetPosition().y);
			mAngle = mBody->GetAngle()*(360.0f/(2.0*PI_CONSTANT));

			mSprite.setPosition(mScreenPosition);
			mSprite.setRotation(mAngle);
		}
	}

}
