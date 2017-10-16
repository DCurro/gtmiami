#ifndef OPENGLGAMEMODE_HPP_
#define OPENGLGAMEMODE_HPP_

#include "GameMode.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#ifdef _WIN64
	#include <GL/gl.h>
	#include <GL/glu.h>
#elif _WIN32
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

class OpenGLGameMode: public GameMode {
public:

	static const GLfloat cube[];

	OpenGLGameMode(GameContext* gameContext);
	virtual ~OpenGLGameMode();

	sf::RenderWindow* window;

	sf::ContextSettings contextSettings;
	sf::Image image;
	GLuint texture;
	sf::Clock clock;

	virtual void initialize() override;
	virtual void handleEvent(sf::Event event) override;
	virtual void update(float timeStep) override;
	virtual void destroy() override;
	virtual void willAppear() override;
	virtual void willDisappear() override;
};

#endif /* OPENGLGAMEMODE_HPP_ */
