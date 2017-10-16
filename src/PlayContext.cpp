#include "PlayContext.hpp"

PlayContext::PlayContext() {
	mGameContext = NULL;
	mPlayGameMode = NULL;
	mWorld = NULL;
	mFactory = NULL;
	mEntityManager = NULL;
	mCamera = NULL;
	mGUIManager = NULL;
	mDistrictManager = NULL;
	mGameTime = NULL;
	mShader = NULL;

	mIsUsingShader = false;
}

PlayContext::~PlayContext() {
}


