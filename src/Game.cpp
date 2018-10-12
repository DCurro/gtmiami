#include "Game.hpp"

#include "GameContext.hpp"
#include "GameMode.hpp"
#include "ResourceManager.hpp"
#include "AudioManager.hpp"
#include "InputHandler.hpp"
#include "PlayGameMode.hpp"
#include "OpenGLGameMode.hpp"
#include "MainMenuGameMode.hpp"

Game::Game() {
	std::srand(time(NULL));

	mSFGUI = new sfg::SFGUI();

	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 32;
	sf::Uint32 style = sf::Style::Titlebar|sf::Style::Close;
	mWindow =  new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "GTMiami pre-demo", style, contextSettings);
	mWindow->setActive(true);
	mWindow->setVerticalSyncEnabled(true);

	mRenderTexture = new sf::RenderTexture();
	mRenderTexture->create(SCREEN_WIDTH,SCREEN_HEIGHT);
	mRenderTexture->setSmooth(true);

	mGameContext = new GameContext();

	ResourceManager* resourceManager = new ResourceManager();
	AudioManager* audioManager = new AudioManager(mGameContext);
	InputHandler* inputHandler = new InputHandler(mGameContext);

	mGameContext->setGame(this);
	mGameContext->setResourceManager(resourceManager);
	mGameContext->setAudioManager(audioManager);
	mGameContext->setInputHandler(inputHandler);

	mGameModeToPush = NULL;
	mShouldPopGameMode = false;
	mGameModeToReset = NULL;

//    this->resetGameMode(new MainMenuGameMode(mGameContext));
    this->resetGameMode(new PlayGameMode(mGameContext));

	//Initialize AudioManager once the build is run
	mGameContext->getAudioManager()->playMusic(ResourceManager::MusicPathResources::hotlineTO_ogg);
}

Game::~Game() {
	delete mGameContext->getResourceManager();
	delete mGameContext->getAudioManager();
	delete mGameContext->getInputHandler();
}

void Game::update(float timeStep) {
	UNUSED(timeStep);

	if(mGameModeToPush) {
		this->pushGameMode(mGameModeToPush);
		mGameModeToPush = NULL;
	}

	if(mShouldPopGameMode) {
		this->popGameMode();
		mShouldPopGameMode = false;
	}

	if(mGameModeToReset) {
		this->resetGameMode(mGameModeToReset);
		mGameModeToReset = NULL;
	}
}

void Game::startWithTimeStep(float timeStep) {
	while (mWindow->isOpen()){
		if( !mWindow->isOpen() ) {
			break;
		}

		mGameContext->getAudioManager()->update(timeStep);
		mGameContext->getInputHandler()->update();

		sf::Event event;
		while (mWindow->pollEvent(event)){
			if(event.type == sf::Event::Closed) {
				this->getWindow()->close();
				break;
			}

			mGameContext->getInputHandler()->handleEvent(event);
			mGameModeStack.top()->handleEvent(event);
		}
		this->update(timeStep);
		mGameModeStack.top()->update(timeStep);
	}
}

void Game::requestPushGameMode(GameMode* gameMode) {
	mGameModeToPush = gameMode;
}

void Game::requestPopGameMode() {
	mShouldPopGameMode = true;
}

void Game::requestResetGameMode(GameMode* gameMode) {
	mGameModeToReset = gameMode;
}

/** pushGameMode
 *
 * sets a game mode to be handle for events.
 */
void Game::pushGameMode(GameMode* gameMode) {
	if(mGameModeStack.size() > 0){
		mGameModeStack.top()->willDisappear();
	}
	gameMode->initialize();
	gameMode->willAppear();
	mGameModeStack.push(gameMode);
}

/** popGameMode
 *
 * removes the top most game mode from the system.
 */
void Game::popGameMode() {
	if(mGameModeStack.size() > 0){
		GameMode* topGameMode = mGameModeStack.top();
		topGameMode->willDisappear();
		topGameMode->destroy();
		mGameModeStack.pop();
		//AudioManager plays music during the intro screen when popGameMode function is called
		mGameContext->getAudioManager()->playMusic(ResourceManager::MusicPathResources::hotlineTO_ogg);

		delete topGameMode;

		mGameModeStack.top()->willAppear();
	}
}

/** resetGameMode
 *
 * removes all other game modes from the stack, and sets this one.
 */
void Game::resetGameMode(GameMode* gameMode) {
	while(mGameModeStack.size() > 0) {
		this->popGameMode();
	}

	gameMode->initialize();
	gameMode->willAppear();
	mGameModeStack.push(gameMode);
}
