#include "NavigationCell.hpp"
#include "CollisionListener.hpp"

#include <string>
#include <algorithm>

#include "common.hpp"
#include "ResourceManager.hpp"
#include "GameContext.hpp"
#include "Camera.hpp"

//#define DEBUG_DRAW_NAV_CELL

NavigationCell::NavigationCell(PlayContext *playContext, b2Vec2 centerPoint, float widthAndHeight)
    : mCenterPoint(centerPoint), mBottomRightPoint(centerPoint+b2Vec2(widthAndHeight/2.0f,widthAndHeight/2.0f))
{
    this->mPlayContext = playContext;
    
	mNorthCell = NULL;
	mSouthCell = NULL;
	mEastCell = NULL;
	mWestCell = NULL;
	mNorthEastCell = NULL;
	mNorthWestCell = NULL;
	mSouthEastCell = NULL;
	mSouthWestCell = NULL;

    mOccupiedTexture = NULL;
}

void NavigationCell::draw(sf::RenderTarget &window) {
#ifdef DEBUG_DRAW_NAV_CELL
    if (this->isOccupied() == true) {
        if (mOccupiedTexture == NULL) {
            mOccupiedTexture = new sf::Texture();
            mOccupiedTexture->loadFromImage(*mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::navCellOccupationHighlight_png));
            
            sf::Vector2f topLeftSpriteCoords = Camera::worldToScreen(sf::Vector2f(this->mCenterPoint.x, this->mCenterPoint.y));
            topLeftSpriteCoords.x = topLeftSpriteCoords.x - ((float)mOccupiedTexture->getSize().x)/2.0f;
            topLeftSpriteCoords.y = topLeftSpriteCoords.y - ((float)mOccupiedTexture->getSize().y)/2.0f;
            
            mSprite.setTexture(*mOccupiedTexture, true);
            mSprite.setPosition(topLeftSpriteCoords);
        }
        
        PlayEntity::draw(window);
    }
#endif
}

NavigationCell::~NavigationCell() {
    if (mOccupiedTexture) {
        delete mOccupiedTexture;
    }
}

std::string NavigationCell::description() {
    std::stringstream stream;
    stream << "[NavigationCell location(" << this->getCenterPoint().x << "," << this->getCenterPoint().y << ")]" << std::endl;
    return stream.str();
}

bool NavigationCell::isOccupied() {
    return (mOccupierEntities.size() > 0);
}

bool NavigationCell::isOccupiedByAnythingOtherThanEntity(PlayEntity* playEntity)
{
    if (mOccupierEntities.size() == 0) {
        return false;
    }
    
    if (mOccupierEntities.size() >= 2) {
        return true;
    }
    
    bool containsPlayEntity = (std::find(mOccupierEntities.begin(), mOccupierEntities.end(), playEntity) != mOccupierEntities.end());
    if (containsPlayEntity == true) {
        return false;
    }

    return true;
}

bool NavigationCell::isOccupiedByEntity(PlayEntity *playEntity) {
    if (std::find(mOccupierEntities.begin(), mOccupierEntities.end(), playEntity) != mOccupierEntities.end()) {
        return true;
    }
    
    return false;
}

void NavigationCell::occupyCell(PlayEntity* playEntity)
{
    this->mOccupierEntities.insert(playEntity);
}

void NavigationCell::unoccupyCell(PlayEntity* entityToRemove)
{
    auto iter = this->mOccupierEntities.find(entityToRemove);
    this->mOccupierEntities.erase(iter, this->mOccupierEntities.end());
    
}

void NavigationCell::clearOccupation() {
    mOccupierEntities.clear();
}

#pragma PlayEntity abstract methods

Animation*NavigationCell::newBulletStruckAnimation() {
    return NULL;
}

void NavigationCell::struckByBulletConsequences(float bulletCalibre) {
    UNUSED(bulletCalibre);
}

void NavigationCell::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
    UNUSED(blastForce);
    UNUSED(damage);
}


