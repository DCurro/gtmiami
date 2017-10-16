#ifndef MAINMENUGAMEMODE_HPP_
#define MAINMENUGAMEMODE_HPP_

#include "GameMode.hpp"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class GameContext;

class MainMenuGameMode: public GameMode {
public:
	MainMenuGameMode(GameContext* gameContext);
	virtual ~MainMenuGameMode();

	sf::RenderWindow* mRenderWindow;

	sfg::Desktop desktop;
	sfg::Window::Ptr window;

	virtual void initialize() override;
	virtual void handleEvent(sf::Event event) override;
	virtual void update(float timeStep) override;
	virtual void destroy() override;
	void willAppear() override;
	virtual void willDisappear() override;

	void onNewGameButtonClicked();
	void onOpenGLDemoButtonClicked();
};

#endif /* MAINMENUGAMEMODE_HPP_ */
