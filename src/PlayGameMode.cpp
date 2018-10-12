#include "PlayGameMode.hpp"

#include <iostream>

#include <SFML/Audio.hpp>
#include <SFGUI/SFGUI.hpp>
#include <Box2D/Box2D.h>

#include "common.hpp"
#include "Game.hpp"
#include "GameContext.hpp"
#include "PlayEntity.hpp"
#include "PlayContext.hpp"
#include "CollisionListener.hpp"
#include "Factory.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "GUIManager.hpp"
#include "DistrictManager.hpp"
#include "Box2DDebugDraw.hpp"
#include "CollisionContactFilter.hpp"
#include "AudioManager.hpp"
#include "InputHandler.hpp"
#include "GameTime.hpp"
#include "ActionTimer.hpp"
#include "Stairwell.hpp"
#include "Grenade.hpp"
#include "Zombie.hpp"
#include "Box2DWorld.hpp"

PlayGameMode::PlayGameMode(GameContext* gameContext) : GameMode(gameContext) {
	mPlayContext = new PlayContext();
	mPlayContext->setGameContext(gameContext);

	mTimeStep = 1.0f/60.0f;

	this->debugDrawer = NULL;
    
	fpsUpdateTimer = 0.0f;
}

PlayGameMode::~PlayGameMode() {
}

void PlayGameMode::onActionTimerRing() {

}

void PlayGameMode::initialize() {
	mActionTimer = new ActionTimer(3.0f, std::bind( &PlayGameMode::onActionTimerRing, this ));

	Game* game = mPlayContext->getGameContext()->getGame();
	mPlayContext->setPlayGameMode(this);

	sf::RenderWindow& window = *game->getWindow();
	sf::RenderTexture& renderTexture = *game->getRenderTexture();

	b2Vec2 gravity(0.0f, 0.0f);
	Box2DWorld* world = new Box2DWorld(gravity);
	CollisionListener* collisionListener = new CollisionListener(); //TODO: clean up memory
	world->SetContactListener(collisionListener);
	CollisionContactFilter* collisionContactFilter = new CollisionContactFilter(); //TODO: clean up memory
	world->SetContactFilter(collisionContactFilter);

	Factory* factory = new Factory();
	EntityManager* entityManager = new EntityManager(mPlayContext);
	Camera* camera = new Camera(sf::Vector2f(0.0f,0.0f));
	GUIManager* guiManager = new GUIManager(mPlayContext);
	DistrictManager* districtManager = new DistrictManager(mPlayContext);
	GameTime* gameTime = new GameTime( mTimeStep*60.0f );
	audioManager = mPlayContext->getGameContext()->getAudioManager();
//	audioManager->spatial();

    debugDrawer = new Box2DDebugDraw(renderTexture, camera);
//    debugDrawer->SetFlags(b2Draw::e_shapeBit);
    debugDrawer->SetFlags(b2Draw::e_shapeBit|b2Draw::e_aabbBit);
//    world->SetDebugDraw( debugDrawer );

    sf::Shader* shader = NULL;
//	if(sf::Shader::isAvailable()){
//		shader = new sf::Shader();
//		shader->loadFromMemory(*resourceManager->getShader(ResourceManager::ShaderResources::shader_frag), sf::Shader::Fragment);
//	}

	mPlayContext->setWorld(world);
	mPlayContext->setFactory(factory);
	mPlayContext->setEntityManager(entityManager);
	mPlayContext->setCamera(camera);
	mPlayContext->setGUIManager(guiManager);
	mPlayContext->setDistrictManager(districtManager);
	mPlayContext->setShader(shader);
	mPlayContext->setGameTime(gameTime);
    mPlayContext->setIsUsingShader(false);

//	mStairwell = new Stairwell(mPlayContext, b2Vec2(9,-9), 0);
//	mStairwell2 = new Stairwell(mPlayContext, b2Vec2(-19,-11), 0);

	fpsUpdateTimer = 0.0f;

	fpsDisplay.setFont(*mPlayContext->getGameContext()->getResourceManager()->getFont(ResourceManager::FontResources::sansation_ttf));
	fpsDisplay.setCharacterSize(12);
	fpsDisplay.setString("FPS: 0");

	//Play music
	audioManager->playMusic(ResourceManager::MusicPathResources::asphaltSaint_v2_ogg);
    
    mPlayContext->getEntityManager()->addEntity(mPlayContext->getFactory()->createHero(mPlayContext, 0, b2Vec2(0,0), 0));
    

    window.clear();     //THESE TWO LINES NEED TO BE HERE TO MAKE RENDERTEXTURE STUFF WORK
    window.display();   //WE DON'T KNOW WHY.  WE ARE USING AN UNSTABLE VERSION OF SFML THOUGH
}

void PlayGameMode::handleEvent(sf::Event event) {
	mPlayContext->getGUIManager()->HandleEvent(event);

	//end processing event early if the mouse press was on a GUI window
	if(event.type == sf::Event::MouseButtonPressed){
		if(mPlayContext->getGUIManager()->clickWasOnGUIWindow()){
			return;
		}
	}

	InputHandler* inputHandler = mPlayContext->getGameContext()->getInputHandler();
	if(inputHandler->r1WasTapped()){
		mPlayContext->getCamera()->zoom(1.1f);
	}
	if(inputHandler->l1WasTapped()){
		mPlayContext->getCamera()->zoom(0.9f);
	}
	if(inputHandler->selectWasTapped_consumeInput()) {
		mPlayContext->getGameContext()->getGame()->requestPopGameMode();
	}

	//TODO: maybe don't be able to close the whole game here.
	switch( event.type ){
		case sf::Event::Closed: {
			mPlayContext->getGameContext()->getGame()->getWindow()->close();
		} break;

		default:
			break;
	}
}

void PlayGameMode::update(float timeStep) {
	UNUSED(timeStep);

	mActionTimer->update(mTimeStep);

	unsigned long startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	unsigned long endTime;

	sf::RenderWindow* window = mPlayContext->getGameContext()->getGame()->getWindow();
	sf::RenderTexture* renderTexture = mPlayContext->getGameContext()->getGame()->getRenderTexture();

	mPlayContext->getGameTime()->update( mTimeStep );

	//Update the timestep for fading music
	audioManager->update(mTimeStep);

	//Box2D simulation settings
	int32 velocityIterations = 4;
	int32 positionIterations = 2;
	mPlayContext->getWorld()->Step(mTimeStep, velocityIterations, positionIterations);

    mPlayContext->getGUIManager()->Update( mTimeStep );
    mPlayContext->getDistrictManager()->update( mTimeStep );

	mPlayContext->getEntityManager()->update( mTimeStep );
//	mStairwell->update( mTimeStep );
//	mStairwell2->update( mTimeStep );

	Camera* camera = mPlayContext->getCamera();
	camera->update(mTimeStep);

	mPlayContext->getDistrictManager()->setVisibleRegion(camera->getCurrentFloor(), camera->getLocation(), camera->getCameraSize());

	renderTexture->setView(mPlayContext->getCamera()->getView());

	sf::Sprite rendTexSprite(renderTexture->getTexture());
	rendTexSprite.setScale( getScaleDifferenceMaintainPerspective(sf::Vector2u(SCREEN_WIDTH,SCREEN_HEIGHT),window->getSize()) );
	sf::Vector2f renderSpriteOffset = getScaledLocationMaintainPerspective(sf::Vector2u(SCREEN_WIDTH,SCREEN_HEIGHT),window->getSize());
	rendTexSprite.setPosition( renderSpriteOffset );

	renderTexture->clear(sf::Color(57,194,157));
		mPlayContext->getDistrictManager()->draw(*renderTexture);
//		mStairwell->draw(*renderTexture);
//		mStairwell2->draw(*renderTexture);
		mPlayContext->getEntityManager()->draw(*renderTexture);
        mPlayContext->getWorld()->DrawDebugData();
    renderTexture->display();

	window->clear(sf::Color::Black);
		window->draw( rendTexSprite );
		mPlayContext->getGUIManager()->Display(*window, *mPlayContext->getGameContext()->getGame()->getSFGUI());
		window->draw( fpsDisplay );
	window->display();

	if(fpsUpdateTimer > 2.0f){
		fpsUpdateTimer = 0.0f;
		endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::stringstream sstm;
		sstm << "FPS: " << (int)(1.0f/((endTime-startTime)/1000.0f));
		fpsDisplay.setString(sstm.str());

	}
	fpsUpdateTimer += mTimeStep;
}

void PlayGameMode::destroy() {
	audioManager->stop();
	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.
	//TODO: call the rest of these
	delete mPlayContext->getEntityManager();
	delete mPlayContext->getFactory(); //must come after delete enityManager
	delete mPlayContext->getCamera();
	delete mPlayContext->getGUIManager();
	delete mPlayContext->getDistrictManager();
	delete mPlayContext->getGameTime();
//	if(shader){
//		delete shader;
//	}

//	delete collisionListener;
	delete debugDrawer;
	delete mPlayContext->getWorld();

	delete mPlayContext;
}

void PlayGameMode::willAppear() {
	mPlayContext->getGUIManager()->setShouldShowAndUpdate(false);
}

void PlayGameMode::willDisappear() {
	mPlayContext->getGUIManager()->setShouldShowAndUpdate(false);
}
