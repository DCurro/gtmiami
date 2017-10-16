#include "AudioManager.hpp"

#include "ResourceManager.hpp"
#include "GameContext.hpp"

AudioManager::AudioManager(GameContext* gameContext) {
	mGameContext = gameContext;

	soundRollingIndex = 0;
	for(int i = 0; i < SOUND_COUNT; i++){
		sf::Sound* sound;
		sound = new sf::Sound();
		soundVector.push_back(sound);
	}

	mTimeAccumulator = 0;
	mCurrentMusic = new sf::Music();
	mFadeOutMusic = new sf::Music();

	mListener = new sf::Listener();
}

AudioManager::~AudioManager(){
	for(int i = 0; i < SOUND_COUNT; i++){
        if (soundVector[i] != NULL) {
            soundVector[i]->stop();
            delete soundVector[i];
        }
	}
    
    if (mFadeOutMusic) {
        mFadeOutMusic->stop();
        delete mFadeOutMusic;
    }
    
    if (mCurrentMusic) {
        mCurrentMusic->stop();
        delete mCurrentMusic;
    }
}

/* update
 *
 *	Keeps track of the time accumulator and fading for the music (in and out)
 *
 */
void AudioManager::update(float timestep){
	mTimeAccumulator += timestep * 100.0f; //Multiply by 20, so it takes 5 seconds to reach max volume

//	std::cout << "Time Accumulator value " << mTimeAccumulator << std::endl;

	//Fade out the last music that is playing
	int fadeOutVolume = std::max(0, MAX_VOLUME - (int)mTimeAccumulator);
	mFadeOutMusic->setVolume(fadeOutVolume);

	//Fade in the new music that is going to be played
	int fadeInMusic = std::min(MAX_VOLUME, (int)mTimeAccumulator);
	mCurrentMusic->setVolume(fadeInMusic);
	if(mTimeAccumulator>0) {
		mCurrentMusic->setLoop(true);
	}else
		mCurrentMusic->setLoop(false);

//	std::cout << fadeOutVolume << " = mFadeOutMusic, " << fadeInMusic << " = mCurrentMusic" << std::endl;
}

/* play
 *
 * plays a sound effect, given the soundBufferEnum.
 */
void AudioManager::play(ResourceManager::SoundResources soundBufferEnum){
	sf::SoundBuffer* soundBuff;
	soundBuff = mGameContext->getResourceManager()->getSoundBuffer(soundBufferEnum);

	soundVector[soundRollingIndex]->setBuffer(*soundBuff);
	soundVector[soundRollingIndex]->play();
	soundRollingIndex++;
	if(soundRollingIndex >= SOUND_COUNT){
		soundRollingIndex = 0;
	}
}

/* playMusic
 *
 * plays a song given the musicPathEnum.
 * To use, audioManager->playMusic(ResourceManager::MusicPathResources:://music title);
 */
void AudioManager::playMusic(int musicPathEnum){
	std::string* musicPath ;
	delete mFadeOutMusic;
	mFadeOutMusic = mCurrentMusic;
	mTimeAccumulator = 0;
	musicPath = mGameContext->getResourceManager()->getMusicPath(musicPathEnum);
	mCurrentMusic = new sf::Music();
	mCurrentMusic->openFromFile(*musicPath);
	mCurrentMusic->setVolume(0);
	mCurrentMusic->play();

//	mCurrentMusic->setPlayingOffset(sf::seconds(14)); //Idea is to have asphaltSaint to loop at 14s


	//Setting the position of where music is being played
//	mCurrentMusic->setPosition(5.0f, 18.0f, 0.0f);
//	mCurrentMusic->setRelativeToListener(true);
//	mCurrentMusic->setMinDistance(5.0f);
//	mCurrentMusic->setAttenuation(10.0f);
}

/* stop
 *
 * Abruptly stops all music and sounds playing
 *
 */
void AudioManager::stop(){
	//stop all music
	mCurrentMusic->stop();
	//stop all sound effects //double check to make sure this work
	for(int i = 0; i < SOUND_COUNT; i++){
		soundVector[i]->stop();
	}

}

/* listener
 *
 * WIP
 * This is for spatialization with regards to the listener position
 *
 */
/*void AudioManager::listenerLocation(sf::Vector2f cameraPosition){
	mListener->setPosition(cameraPosition.x,cameraPosition.y, 0.0f);
}*/
void AudioManager::spatial(float x, float y){
	mListener->setPosition(x, y, 0.0f);
	mListener->setDirection(1.0f, 0.0f, 0.0f);
	mListener->setGlobalVolume(100);

}

