#include "GUIModeTracePlaceTiles.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "DistrictManager.hpp"
#include "GUIManager.hpp"
#include "Camera.hpp"
#include "District.hpp"
#include "Game.hpp"
#include "ButtonGUI.hpp"


GUIModeTracePlaceTiles::GUIModeTracePlaceTiles(PlayContext* playContext, std::string categoryString, int tileSelectedID) : GUIMode(playContext), mCategoryString(categoryString), mTileSelectedID(tileSelectedID) {
	mIsSelecting = false;
}

void GUIModeTracePlaceTiles::initialize(){
	mPlayContext->getDistrictManager()->setShouldShowHighlightedTile(true);

	//sf::Vector2f mousePosition( sf::Vector2f::Vector2(sf::Mouse::getPosition(*mPlayContext->getGameContext()->getGame()->getWindow())) );
	sf::Vector2i mousePositionI = sf::Mouse::getPosition( *mPlayContext->getGameContext()->getGame()->getWindow() );
	sf::Vector2f mousePosition( mousePositionI.x, mousePositionI.y );
	mPlayContext->getDistrictManager()->highlightTileAtWorldPoint(mPlayContext->getCamera()->screenToWorld_relative( mousePosition ));

	mPlayContext->getGUIManager()->setHintText("Drag cursor along tiles to set.  ESC to stop.");
}

void GUIModeTracePlaceTiles::destroy() {
	this->mPlayContext->getDistrictManager()->setShouldShowHighlightedTile(false);
	mPlayContext->getGUIManager()->setHintText("");
}

GUIModeTracePlaceTiles::~GUIModeTracePlaceTiles() {
}

void GUIModeTracePlaceTiles::handleEvent(sf::Event event) {
	if(mPlayContext->getGUIManager()->clickWasOnGUIWindow()){
		return;
	}

	switch( event.type ){
		case sf::Event::MouseMoved : {
			Camera* camera = mPlayContext->getCamera();
			sf::Vector2f mouseCoords(event.mouseMove.x,event.mouseMove.y);
			if(mIsSelecting){
				mStartLocation = mEndLocation;
				mEndLocation = camera->screenToWorld_relative(mouseCoords);
				this->setTiles();
			} else {
				mStartLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
				mEndLocation = mStartLocation;
			}
			mPlayContext->getDistrictManager()->highlightTileAtWorldPoint(camera->screenToWorld_relative(mouseCoords));
		} break;

		case sf::Event::MouseButtonPressed : {
			sf::Vector2f mouseCoords(event.mouseButton.x,event.mouseButton.y);
			mStartLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
			mEndLocation = mStartLocation;
			this->setTiles();
			mIsSelecting = true;
		} break;

		case sf::Event::MouseButtonReleased : {
			if(mIsSelecting){
				sf::Vector2f mouseCoords(event.mouseButton.x,event.mouseButton.y);
				mEndLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
				mStartLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
				this->setTiles();
				mIsSelecting = false;
			}
		} break;

		default:
			break;
	}
}

void GUIModeTracePlaceTiles::setTiles() {
	float startX = mStartLocation.x;
	float startY = mStartLocation.y;
	float endX = mEndLocation.x;
	float endY = mEndLocation.y;
	float lineLength = std::sqrt( (startX-endX)*(startX-endX) + (startY-endY)*(startY-endY) );

	float normalX = (endX-startX)/lineLength;
	float normalY = (endY-startY)/lineLength;

	float xDelta = normalX * (District::METERES_PER_TILE/2.0f);
	float yDelta = normalY * (District::METERES_PER_TILE/2.0f);

	float distance = std::sqrt( (startX-endX)*(startX-endX) + (startY-endY)*(startY-endY) );

	int floor = mPlayContext->getCamera()->getCurrentFloor();
	while(distance > (District::METERES_PER_TILE/2.0f)){
		this->setTile(floor, sf::Vector2f(startX,startY));
		startX+=xDelta;
		startY+=yDelta;
		distance = std::sqrt( (startX-endX)*(startX-endX) + (startY-endY)*(startY-endY) );;
	}

	//may get missed by the iteration
	this->setTile(floor, mStartLocation);
	this->setTile(floor, mEndLocation);
}

//TODO: unify the tile picker classes because they have a lot in common: like this method!
void GUIModeTracePlaceTiles::setTile(int floor, sf::Vector2f location) {
	if(mCategoryString.compare( CATEGORY_WALL ) == 0){
		mPlayContext->getDistrictManager()->setWall(floor, location, mTileSelectedID);
	} else if(mCategoryString.compare( CATEGORY_INTERIOR ) == 0) {
		mPlayContext->getDistrictManager()->setTile(floor, location, mTileSelectedID, mCategoryString);
	} else {
		//assume it's CATEGORY_ROAD
		mPlayContext->getDistrictManager()->setTile(floor, location, mTileSelectedID, mCategoryString);
	}
}

std::string GUIModeTracePlaceTiles::description() {
	return std::string("");
}
