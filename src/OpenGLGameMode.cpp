#include "OpenGLGameMode.hpp"

#include <iostream>
#include <vector>

#include "GameContext.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"

const GLfloat OpenGLGameMode::cube[] = {
	//positions    // texture coordinates
	-20, -20, -20,  0, 0,
	-20,  20, -20,  1, 0,
	-20, -20,  20,  0, 1,
	-20, -20,  20,  0, 1,
	-20,  20, -20,  1, 0,
	-20,  20,  20,  1, 1,

	 20, -20, -20,  0, 0,
	 20,  20, -20,  1, 0,
	 20, -20,  20,  0, 1,
	 20, -20,  20,  0, 1,
	 20,  20, -20,  1, 0,
	 20,  20,  20,  1, 1,

	-20, -20, -20,  0, 0,
	 20, -20, -20,  1, 0,
	-20, -20,  20,  0, 1,
	-20, -20,  20,  0, 1,
	 20, -20, -20,  1, 0,
	 20, -20,  20,  1, 1,

	-20,  20, -20,  0, 0,
	 20,  20, -20,  1, 0,
	-20,  20,  20,  0, 1,
	-20,  20,  20,  0, 1,
	 20,  20, -20,  1, 0,
	 20,  20,  20,  1, 1,

	-20, -20, -20,  0, 0,
	 20, -20, -20,  1, 0,
	-20,  20, -20,  0, 1,
	-20,  20, -20,  0, 1,
	 20, -20, -20,  1, 0,
	 20,  20, -20,  1, 1,

	-20, -20,  20,  0, 0,
	 20, -20,  20,  1, 0,
	-20,  20,  20,  0, 1,
	-20,  20,  20,  0, 1,
	 20, -20,  20,  1, 0,
	 20,  20,  20,  1, 1
};

OpenGLGameMode::OpenGLGameMode(GameContext* gameContext) : GameMode(gameContext) {
	texture = 0;
	window = NULL;
}

OpenGLGameMode::~OpenGLGameMode() {

}

void OpenGLGameMode::initialize(){
//	Request a 32-bits depth buffer when creating the window
//	contextSettings.depthBits = 32;
	window = mGameContext->getGame()->getWindow();

//	 Load an OpenGL texture.
//	 We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
//	 but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
	texture = 0;
	{
		image = *mGameContext->getResourceManager()->getImage(ResourceManager::ImageResources::texture_png);
		std::cout << image.getSize().x << "--" << image.getSize().y << std::endl;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 24, 24, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		for(unsigned int i=0; i<image.getSize().x*image.getSize().x*4; i++) {
			int pixel = image.getPixelsPtr()[i];
			std::cout << pixel << " ";
			if((i+1)%4 == 0) {
				std::cout << "pixel " <<(i-1)/4;
				std::cout << std::endl;
			}
		}

	}

//	 Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);

//	 Disable lighting
	glDisable(GL_LIGHTING);

//	 Configure the viewport (the same size as the window)
	glViewport(0, 0, window->getSize().x, window->getSize().y);

//	 Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = static_cast<float>(window->getSize().x) / window->getSize().y;
	glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

//	 Bind the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

//	 Enable position and texture coordinates vertex components
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 5 * sizeof(const GLfloat), cube);
	glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(const GLfloat), cube + 3);

//	 Disable normal and color vertex components
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void OpenGLGameMode::handleEvent(sf::Event event) {
    UNUSED(event);
}

void OpenGLGameMode::update(float timeStep){
    UNUSED(timeStep);
    
	window->pushGLStates();
	window->clear();
	window->popGLStates();

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// We get the position of the mouse cursor, so that we can move the box accordingly
	float x =  sf::Mouse::getPosition(*window).x * 200.f / window->getSize().x - 100.f;
	float y = -sf::Mouse::getPosition(*window).y * 200.f / window->getSize().y + 100.f;

	// Apply some transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, -100.f);
//	glRotatef(clock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f);
//	glRotatef(clock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f);
//	glRotatef(clock.getElapsedTime().asSeconds() * 90.f, 0.f, 0.f, 1.f);

	//draw the cube
	glDrawArrays(GL_TRIANGLES, 0, 36);

	window->display();
}

void OpenGLGameMode::destroy() {
	// Don't forget to destroy our texture
	glDeleteTextures(1, &texture);

	std::cout << "OpenGL demo ended succesfully" << std::endl;
}

void OpenGLGameMode::willAppear() {

}
void OpenGLGameMode::willDisappear() {

}
