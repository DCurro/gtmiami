#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

class PlayContext;

/**
 * \brief Creates an animation for the sprite-sheet texture
 *
 * The given texture must have animation cells from left to right on one strip of image.
 * The give texture must have cells of square shape.
 * normalMap is not necessary.
 * framesPerSecond controls the speed at which the animation plays.
 **/

class Animation {
private:
	float mTimer;
	int mCurrentFrameIndex;
	int mFrameCount;
	float mSecondsPerFrame;
	float mFullAnimationTime;
	bool mHasNormalMap;
	std::vector<sf::Texture*>* mTextureFrames;
	std::vector<sf::Texture*>* mNormapMapFrames;

	bool mLooped;
	sf::Vector2i mStartVec;
	sf::Vector2i mDimensionsVec;
	float mFPS;

public:
	Animation(bool looped, float framesPerSecond, sf::Image* image, sf::Image* normalMap=NULL);
	virtual ~Animation();

	void update(float timeStep);

	bool hasCompletedAnimation();

	/**
	  *   \return The current sf::Texture, which is a sub-region of the original texture
	  *
	**/
	sf::Texture* getCurrentFrame();
	sf::Texture* getCurrentNormalMapFrame();

	bool hasNormalMap();
};

#endif /* ANIMATION_HPP_ */
