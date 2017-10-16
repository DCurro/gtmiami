#include "MainMenuGameMode.hpp"

#include <stddef.h>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Game.hpp"
#include "GameContext.hpp"
#include "PlayGameMode.hpp"
#include "OpenGLGameMode.hpp"
#include "ResourceManager.hpp"

MainMenuGameMode::MainMenuGameMode(GameContext* gameContext) : GameMode(gameContext) {
	mRenderWindow = NULL;
}

MainMenuGameMode::~MainMenuGameMode() {
}

void MainMenuGameMode::initialize() {
	mRenderWindow = mGameContext->getGame()->getWindow();

	window = sfg::Window::Create();
	window->SetTitle( "Title" );
	window->SetStyle(sfg::Window::Style::BACKGROUND);
	window->SetRequisition(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	desktop.Add(window);

	sfg::Box::Ptr containerBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	sfg::Box::Ptr logoBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sfg::Box::Ptr buttonsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 15.0f);

	sfg::Image::Ptr logo = sfg::Image::Create(*mGameContext->getResourceManager()->getImage(ResourceManager::ImageResources::TitleLogo_png));

	sfg::Button::Ptr newGameButton = sfg::Button::Create("NEW GAME");
	newGameButton->GetSignal( sfg::Widget::OnMouseLeftPress ).Connect( std::bind( &MainMenuGameMode::onNewGameButtonClicked, this) );
	sfg::Button::Ptr settingsButton = sfg::Button::Create("OPENGL DEMO");
	settingsButton->GetSignal( sfg::Widget::OnMouseLeftPress ).Connect( std::bind( &MainMenuGameMode::onOpenGLDemoButtonClicked, this) );

	logoBox->Pack(logo);
	buttonsBox->Pack(newGameButton);
	buttonsBox->Pack(settingsButton);

	containerBox->Pack(logoBox);
	containerBox->Pack(buttonsBox);
	window->Add(containerBox);

	mRenderWindow->clear();     //THESE TWO LINES NEED TO BE HERE TO MAKE RENDERTEXTURE STUFF WORK
	mRenderWindow->display();   //WE DON'T KNOW WHY.  WE ARE USING AN UNSTABLE VERSION OF SFML THOUGH

}

void MainMenuGameMode::handleEvent(sf::Event event) {
//	switch( event.type ){
//		case sf::Event::KeyPressed: {
//			if(event.key.code == sf::Keyboard::Q){
//				mGameContext->getGame()->popGameMode();
//			}
//		} break;
//	}
	desktop.HandleEvent(event);
}

void MainMenuGameMode::update(float timeStep) {
	desktop.Update(timeStep);


	sf::RenderWindow* window = mGameContext->getGame()->getWindow();
	window->clear(sf::Color::Black);
		mGameContext->getGame()->getSFGUI()->Display(*window);

		sf::Text text;
		text.setFont(*mGameContext->getResourceManager()->getFont(ResourceManager::FontResources::sansation_ttf));
		text.setCharacterSize(1);
		text.setString("-");
		window->draw(text);

	window->display();
}

void MainMenuGameMode::destroy() {

}

void MainMenuGameMode::willAppear() {
	window->Show(true);
}

void MainMenuGameMode::willDisappear() {
	window->Show(false);
}


void MainMenuGameMode::onNewGameButtonClicked(){
	mGameContext->getGame()->requestPushGameMode(new PlayGameMode(mGameContext));
}

void MainMenuGameMode::onOpenGLDemoButtonClicked() {
	mGameContext->getGame()->requestPushGameMode(new OpenGLGameMode(mGameContext));
}
