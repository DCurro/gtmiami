#include "GUIModeBoxPlaceTiles.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "DistrictManager.hpp"
#include "GUIManager.hpp"
#include "Camera.hpp"
#include "District.hpp"
#include "Game.hpp"
#include "ButtonGUI.hpp"

GUIModeBoxPlaceTiles::GUIModeBoxPlaceTiles(PlayContext* playContext, std::string categoryString, int tileSelectedID) : GUIMode(playContext), mCategoryString(categoryString), mTileSelectedID(tileSelectedID) {
	mIsSelecting = false;
}

void GUIModeBoxPlaceTiles::initialize() {
	mPlayContext->getDistrictManager()->setShouldShowHighlightedTile(true);

	sf::Vector2i mousePositionI = sf::Mouse::getPosition( *mPlayContext->getGameContext()->getGame()->getWindow() );
	mPlayContext->getDistrictManager()->highlightTileAtWorldPoint( mPlayContext->getCamera()->screenToWorld_relative( sf::Vector2f( mousePositionI.x, mousePositionI.y ) ));

	mPlayContext->getGUIManager()->setHintText("Drag and release to place tiles.  ESC to stop.");
}

void GUIModeBoxPlaceTiles::destroy() {
	this->mPlayContext->getDistrictManager()->setShouldShowBoxHighlight(false);
	this->mPlayContext->getDistrictManager()->setShouldShowHighlightedTile(false);
	mPlayContext->getDistrictManager()->resetHighlightTileIndicatorSize();

	mPlayContext->getGUIManager()->setHintText("");
}

GUIModeBoxPlaceTiles::~GUIModeBoxPlaceTiles() {
}

void GUIModeBoxPlaceTiles::handleEvent(sf::Event event) {
	if(mPlayContext->getGUIManager()->clickWasOnGUIWindow()){
		return;
	}

	switch( event.type ){
		case sf::Event::MouseMoved : {
			Camera* camera = mPlayContext->getCamera();
			sf::Vector2f mouseCoords(event.mouseMove.x,event.mouseMove.y);
			if(mIsSelecting){
				mPlayContext->getDistrictManager()->setShouldShowBoxHighlight(true);

				mEndLocation = camera->screenToWorld_relative(mouseCoords);

				float startX = minX(mStartLocation, mEndLocation);
				float endX = maxX(mStartLocation, mEndLocation);
				float startY = minY(mStartLocation, mEndLocation);
				float endY = maxY(mStartLocation, mEndLocation);
				sf::FloatRect rect(startX, startY, endX-startX, endY-startY);

				mPlayContext->getDistrictManager()->highlightTilesInRectangle(rect);
			} else {
				mPlayContext->getDistrictManager()->highlightTileAtWorldPoint(camera->screenToWorld_relative(mouseCoords));
			}
		} break;

		case sf::Event::MouseButtonPressed : {
			if(event.mouseButton.button == sf::Mouse::Button::Left){
				sf::Vector2f mouseCoords(event.mouseButton.x,event.mouseButton.y);
				mStartLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
				mIsSelecting = true;
			}
		} break;

		case sf::Event::MouseButtonReleased : {
			if(event.mouseButton.button == sf::Mouse::Button::Left){
				if(mIsSelecting){
					sf::Vector2f mouseCoords(event.mouseButton.x,event.mouseButton.y);
					mEndLocation = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
					this->setTiles();
					mIsSelecting = false;
					mPlayContext->getDistrictManager()->setShouldShowBoxHighlight(false);
					sf::Vector2f mouseWorldCoords = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
					mPlayContext->getDistrictManager()->resetHighlightTileIndicatorSize();
					mPlayContext->getDistrictManager()->highlightTileAtWorldPoint(mouseWorldCoords);
				}
			}
		} break;

		default:
			break;
	}
}

void GUIModeBoxPlaceTiles::setTiles() {
	float startX = minX(mStartLocation, mEndLocation);
	float endX = maxX(mStartLocation, mEndLocation);
	float startY = minY(mStartLocation, mEndLocation);
	float endY = maxY(mStartLocation, mEndLocation);

	int floor = mPlayContext->getCamera()->getCurrentFloor();

	for(float y = startY; y<=endY; y+=District::METERES_PER_TILE){
		for(float x = startX; x<=endX; x+=District::METERES_PER_TILE){
			this->setTile(floor, sf::Vector2f(x,y));
		}
	}

	//bottom and left edge (which may get left out of the +=District::METERS_PER_TILE calculation)
	//bottom edge
	for(float x = startX; x<=endX; x+=District::METERES_PER_TILE){
		this->setTile(floor, sf::Vector2f(x,endY));
	}
	//right edge
	for(float y = startY; y<=endY; y+=District::METERES_PER_TILE){
		this->setTile(floor, sf::Vector2f(endX,y));
	}
	//bottom right corner
	this->setTile(floor, sf::Vector2f(endX,endY));


}

void GUIModeBoxPlaceTiles::setTile(int floor, sf::Vector2f location) {
	if(mCategoryString.compare( CATEGORY_WALL ) == 0){
		mPlayContext->getDistrictManager()->setWall(floor, location, mTileSelectedID);
	} else if(mCategoryString.compare( CATEGORY_INTERIOR ) == 0) {
		mPlayContext->getDistrictManager()->setTile(floor, location, mTileSelectedID, mCategoryString);
	} else {
		//assume it's CATEGORY_ROAD
		mPlayContext->getDistrictManager()->setTile(floor, location, mTileSelectedID, mCategoryString);
	}
}

std::string GUIModeBoxPlaceTiles::description() {
	return std::string("");
}

