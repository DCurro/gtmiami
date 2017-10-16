#include "Animation.hpp"

#include "PlayContext.hpp"

Animation::Animation(bool looped, float framesPerSecond, sf::Image* image, sf::Image* normalMap)
	:mLooped(looped), mFPS(framesPerSecond)
{
	assert( (float)image->getSize().x/(float)image->getSize().y == ((int)((float)image->getSize().x/(float)image->getSize().y))
			&& "Animation only works on images who's width is divisible by it's height (square cells in an animation strip)" );

	mTimer = 0;
	mCurrentFrameIndex = 0;
	mFrameCount = std::max( (int)(image->getSize().x/image->getSize().y), 1 );
	mSecondsPerFrame = 1.0f/framesPerSecond;
	mFullAnimationTime = mSecondsPerFrame*mFrameCount;

	normalMap ? mHasNormalMap=true : mHasNormalMap=false;

	mTextureFrames = new std::vector<sf::Texture*>();
    
	mNormapMapFrames = new std::vector<sf::Texture*>();

	for(unsigned int i=0; i<image->getSize().x; i+=image->getSize().y ){
		sf::Texture* texture = new sf::Texture();
		texture->loadFromImage(*image, sf::IntRect(i, 0, image->getSize().y, image->getSize().y));
		mTextureFrames->push_back(texture);

		if(mHasNormalMap){
			sf::Texture* normal = new sf::Texture();
			normal->loadFromImage(*normalMap, sf::IntRect(i, 0, image->getSize().y, image->getSize().y));
			mNormapMapFrames->push_back(texture);
		}
	}
}

Animation::~Animation() {
	for(auto texture : *mTextureFrames){
		delete texture;
	}
	delete mTextureFrames;

	for(auto texture : *mNormapMapFrames){
		delete texture;
	}
	delete mNormapMapFrames;
}

void Animation::update(float timeStep){
	if(mTimer > mFullAnimationTime && mLooped==true){
		mTimer -= mFullAnimationTime;
	}
	mCurrentFrameIndex = mFPS*mTimer;
	mTimer += timeStep;

	if(mLooped==false && mCurrentFrameIndex >= mFrameCount){
		mCurrentFrameIndex=mFrameCount-1;
	}

	assert(mCurrentFrameIndex >= 0);
	assert(mCurrentFrameIndex < mFrameCount);
}

bool Animation::hasCompletedAnimation() {
	if(mCurrentFrameIndex == mFrameCount-1) {
		return true;
	} else {
		return false;
	}
}

sf::Texture* Animation::getCurrentFrame(){
	return mTextureFrames->at(mCurrentFrameIndex);
}

sf::Texture* Animation::getCurrentNormalMapFrame(){
	return mNormapMapFrames->at(mCurrentFrameIndex);
}

bool Animation::hasNormalMap(){
	return mHasNormalMap;
}
