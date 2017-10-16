#include "GUIManager.hpp"

#import "Game.hpp"
#include "GUIMode.hpp"
#include "TilePickerViewController.hpp"
#include "HintViewController.hpp"
#include "TimeStepSliderViewController.hpp"
#include "ObjectInspectorViewController.hpp"
#include "FloorSelectorViewController.hpp"
#include "PooledPlayEntityPickerViewController.hpp"

GUIManager::GUIManager(PlayContext* playContext) : mPlayContext(playContext) {
	mShouldShowAndUpdate = false;
	mGUIMode = NULL;

	//generic view controllers
    auto tilePickerViewController = new TilesPickerViewController(playContext);
    tilePickerViewController->getView()->SetPosition(sf::Vector2f(0,0));
    
    auto timeStepSliderViewController = new TimeStepSliderViewController(playContext);
    timeStepSliderViewController->getView()->SetPosition(sf::Vector2f(196,0));
    
    auto objectInpectorViewController = new ObjectInspectorViewController(playContext);
    objectInpectorViewController->getView()->SetPosition(sf::Vector2f(SCREEN_WIDTH-222, 0));
    
    auto floorSelectorViewController = new FloorSelectorViewController(playContext);
    floorSelectorViewController->getView()->SetPosition(sf::Vector2f(196,661));
    
    auto pooledPlayEntityPickerViewController = new PooledPlayEntityPickerViewController(playContext);
    pooledPlayEntityPickerViewController->getView()->SetPosition(sf::Vector2f(200,0));
    
    //special view controllers
    mHintViewController = new HintViewController(playContext);
    mHintViewController->getView()->SetPosition(sf::Vector2f(0,SCREEN_HEIGHT-40));

    this->addViewController(tilePickerViewController);
	this->addViewController(timeStepSliderViewController);
	this->addViewController(objectInpectorViewController);
	this->addViewController(floorSelectorViewController);
    this->addViewController(pooledPlayEntityPickerViewController);

	this->addViewController(mHintViewController);
}

GUIManager::~GUIManager() {
	for(auto vc : viewControllers){
		delete vc;
	}
}

void GUIManager::Update(float deltaTime){
	if(!mShouldShowAndUpdate) {
		return;
	}

	for(ViewController* vc : viewControllers) {
		if(vc->getIsSetup() == false){
			vc->setupGUI();
			this->Add(vc->getView());
		}
	}

	for(auto vc : viewControllers) {
		if(vc->getIsSetup()){
			vc->update(deltaTime);
			vc->setClicked(false);
		}
	}

	sfg::Desktop::Update(deltaTime);
}

void GUIManager::Display(sf::RenderWindow& target, sfg::SFGUI& sfgui){
	if(mShouldShowAndUpdate) {
		sfgui.Display(target);
	}
}

void GUIManager::HandleEvent( const sf::Event& event ) {
	sfg::Desktop::HandleEvent(event);

	switch( event.type ){
		case sf::Event::KeyPressed : {
			if(event.key.code == sf::Keyboard::Key::G){
				mShouldShowAndUpdate = !mShouldShowAndUpdate;
				if(!mShouldShowAndUpdate){
					this->clearMode();
				}
			} else if(event.key.code == sf::Keyboard::Key::Escape){
				if(mShouldShowAndUpdate){
					this->clearMode();
				}
			}
		} break;

		default:
			break;
	}

	if(mShouldShowAndUpdate){
		if(mGUIMode) {
			mGUIMode->handleEvent(event);
		}
		for(auto vc : viewControllers){
			vc->handleEvent(event);
		}
	}
}

void GUIManager::addViewController(ViewController* viewController){
	viewControllers.push_back(viewController);
}

/* clickWasOnGUIWindow
 *
 * Determines if a mouse press occured on any containing
 * GUI windows.
 */
bool GUIManager::clickWasOnGUIWindow(){
	bool clickWasOnGUIWindow = false;

	for(auto vc : this->viewControllers){
		if(vc->clickWasOnGUIWindow()){
			clickWasOnGUIWindow = true;
			break;
		}
	}
	return clickWasOnGUIWindow;
}

/* setMode
 *
 * Destroys the previous mode and initializes the new one.
 */
void GUIManager::setMode(GUIMode* guiMode) {
	this->clearMode();
	if(guiMode){
		mGUIMode = guiMode;
		mGUIMode->initialize();
	}
}


void GUIManager::clearMode() {
	if(mGUIMode) {
		mGUIMode->destroy();
		delete mGUIMode;
	}
	mGUIMode = NULL;
	mHintViewController->setDescription("Right-click on an object to inspect.  Press G to toggle the GUI on and off.");
}

void GUIManager::setHintText(std::string hintString) {
	mHintViewController->setDescription(hintString);
}
