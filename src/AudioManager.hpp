#ifndef AUDIOMANAGER_HPP_
#define AUDIOMANAGER_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Audio.hpp>

#include "ResourceManager.hpp"
#include "common.hpp"

class ResourceManager;
class GameContext;

class AudioManager {
private:
	ACCESSOR(GameContext*, GameContext);

	const int MAX_VOLUME = 100;
	const int SOUND_COUNT = 25;
	int soundRollingIndex;
	std::vector<sf::Sound*> soundVector;

	sf::Music* mCurrentMusic; //Initialize music value
	sf::Music* mFadeOutMusic;

	float mTimeAccumulator;

	sf::Listener* mListener;

public:
	AudioManager(GameContext* gameContext);

	virtual ~AudioManager();

	virtual void update(float timestep);
	virtual void play(ResourceManager::SoundResources soundResource);
	virtual void playMusic(int musicPathEnum);
	virtual void stop();
//	virtual void listenerLocation(sf::Vector2f cameraPosition);
	virtual void spatial(float x, float y);
};

#endif /* AUDIOMANAGER_HPP_ */
