#include "Camera.hpp"

#include "Game.hpp"
#include "PooledPlayEntity.hpp"


Camera::Camera(sf::Vector2f location) {
	mZoomLevel = 1.0f;
	mFocus=NULL;
	mView = sf::View(location, sf::Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT));
	sf::Vector2i screenCoords(0,0);// = this->worldToScreen(location);
	sf::Vector2f screenCoordsFloat(screenCoords.x, screenCoords.y);
	mView.setCenter(screenCoordsFloat);
}

Camera::~Camera() {

}

void Camera::update(float timeStep){
	UNUSED(timeStep);

	if(mFocus!=NULL && this->isValid(mFocus)){
		float x = mFocus->getBody()->GetPosition().x;
		float y = mFocus->getBody()->GetPosition().y;
		this->setLocation(mFocus->getCurrentFloor(), sf::Vector2f(x,y));
	}
}

sf::View Camera::getView(){
	return mView;
}

void Camera::setLocation(int floor, sf::Vector2f location){
	mCurrentFloor = floor;
	sf::Vector2f screenCoords = this->worldToScreen(location);
	sf::Vector2f camCenter;
	camCenter.x = screenCoords.x;
	camCenter.y = screenCoords.y;
	mView.setCenter(camCenter);
}

sf::Vector2f Camera::getLocation(){
	return this->screenToWorld( mView.getCenter() );
}

void Camera::zoom(float zoomFactor){
	mZoomLevel *= zoomFactor;
	mView.zoom(zoomFactor);
}

void Camera::setZoomLevel(float zoomLevel){
	mView.zoom(1.0f/mZoomLevel); //restore the zoom to 1.0f

	mZoomLevel = zoomLevel;
	mView.zoom(mZoomLevel);
}

float Camera::getZoomLevel(){
	return mZoomLevel;
}


/*    Convert from absolute screen coordinates to absolute world coordinates
 *     to absolute world coordinates
 *
 */
sf::Vector2f Camera::screenToWorld(sf::Vector2f screenCoords){
	float x = Camera::METERS_PER_PIXEL_AT_DEFAULT_VIEW * (screenCoords.x);
	float y = Camera::METERS_PER_PIXEL_AT_DEFAULT_VIEW * (screenCoords.y);
	return sf::Vector2f(x,y);
}

/*    Convert from relative screen coordinates to world coordinates
 *     to absolute world coordinates
 *
 */
sf::Vector2f Camera::screenToWorld_relative(sf::Vector2f screenCoords){
	screenCoords.x = (screenCoords.x - (SCREEN_WIDTH/2.0f))*mZoomLevel;
	screenCoords.y = (screenCoords.y - (SCREEN_HEIGHT/2.0f))*mZoomLevel;
	sf::Vector2f conversion = this->screenToWorld(screenCoords);
	conversion.x += this->getLocation().x;
	conversion.y += this->getLocation().y;
	return conversion;

}

/*    Convert from absolute world coordinates to absolute screen coordinates.
 */
sf::Vector2f Camera::worldToScreen(sf::Vector2f worldCoords){
	int x = Camera::PIXELS_PER_METER_AT_DEFAULT_VIEW * (worldCoords.x);
	int y = Camera::PIXELS_PER_METER_AT_DEFAULT_VIEW * (worldCoords.y);
	return sf::Vector2f(x,y);
}

sf::Vector2f Camera::getCameraSize(){
	return Camera::screenToWorld(this->getView().getSize());
}

void Camera::requestImmediateFocus(PooledPlayEntity* focalEntity) {
	if(focalEntity) {
		mFocus = focalEntity;
		this->registerPooledPlayEntity(mFocus);
	}
}

// PlayEntity

Animation* Camera::newBulletStruckAnimation() {
	return NULL;
}

void Camera::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre)
}

void Camera::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}
