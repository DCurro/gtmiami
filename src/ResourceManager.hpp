#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <map>

#include "extlibs/rapidjson/document.h"		// rapidjson's DOM-style API
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



//http://stackoverflow.com/questions/201593/is-there-a-simple-script-to-convert-c-enum-to-string/11586083#11586083
/*****************************************************************
 * 			THIS IS WHERE YOU ADD RESOURCES
 *****************************************************************/

	//Image Resources
	//http://i.imgur.com/oklNT8p.png (explosion.png)
    //https://www.google.ca/url?sa=i&rct=j&q=&esrc=s&source=images&cd=&cad=rja&uact=8&ved=0CAQQjBw&url=http%3A%2F%2Fforumfiles.thegamecreators.com%2Fdownload%2F2279045&ei=aZdgVae8CYaxsAS1-IHoAw&bvm=bv.93990622,d.cWc&psig=AFQjCNEFYKziETdF21s-FJi86VTE50sQAA&ust=1432479951707890 (zombie)
	#define IMAGE_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(tilemap, png) \
		ADD_RESOURCE(interiorTilemap, png) \
		ADD_RESOURCE(wallTilemap, png) \
		ADD_RESOURCE(demon, png) \
		ADD_RESOURCE(demon_nrm, png) \
		ADD_RESOURCE(reticle, png) \
		ADD_RESOURCE(car, png) \
		ADD_RESOURCE(car_nrm, png) \
		ADD_RESOURCE(TitleLogo, png) \
		ADD_RESOURCE(texture, png) \
		ADD_RESOURCE(towTruck, png) \
		ADD_RESOURCE(HeroWalkCycle, png) \
		ADD_RESOURCE(HeroIdleCycle, png) \
		ADD_RESOURCE(HeroDeadCycle, png) \
		ADD_RESOURCE(BadGuy, png)	\
		ADD_RESOURCE(carV2, png) \
		ADD_RESOURCE(floorRevolver, png) \
		ADD_RESOURCE(ak47, png) \
		ADD_RESOURCE(greyBulletStrikeAnimation, png) \
		ADD_RESOURCE(bloodBulletStrikeAnimation, png) \
		ADD_RESOURCE(badGuyDead, png) \
		ADD_RESOURCE(turretTop, png) \
		ADD_RESOURCE(stairwell, png) \
		ADD_RESOURCE(rocketLauncher, png) \
		ADD_RESOURCE(rocket, png) \
		ADD_RESOURCE(sparkBulletStrikeAnimation, png) \
		ADD_RESOURCE(grenade, png) \
		ADD_RESOURCE(grenadeStash, png) \
		ADD_RESOURCE(explosion, png) \
		ADD_RESOURCE(bunnyIdleCycle, png) \
		ADD_RESOURCE(bunnyWalkCycle, png) \
		ADD_RESOURCE(bunnyDeadCycle, png) \
		ADD_RESOURCE(spiritWalkCycle, png) \
        ADD_RESOURCE(navCellOccupationHighlight, png) \
        ADD_RESOURCE(zombieWalkCycle, png) \
        ADD_RESOURCE(zombieIdleCycle, png)

	//Sound Effect (buffer) Resources
	//https://www.freesound.org/people/GregsMedia/sounds/150137/ (gunBlast.wav)
	#define SOUND_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(no, sound) \
		ADD_RESOURCE(demonAttack, wav) \
		ADD_RESOURCE(gunShot, wav)	\
		ADD_RESOURCE(gunShot_mono, wav) \
		ADD_RESOURCE(gunBlast, wav) \
		ADD_RESOURCE(rocketBlast, wav) \
		ADD_RESOURCE(explosion, wav) \
		ADD_RESOURCE(punch, wav)

	//Music (path only) Resources
	#define MUSICPATH_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(cheer, ogg) \
		ADD_RESOURCE(arenaSong, ogg)	\
		ADD_RESOURCE(hotlineTO, ogg) \
		ADD_RESOURCE(asphaltSaint_v2, ogg)

	//Font Resources
	#define FONT_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(sansation, ttf)

	//Shader Resources
	#define SHADER_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(shader, frag)

	//JSON Resources
	#define JSON_RESOURCE_ENUM(ADD_RESOURCE) \
		ADD_RESOURCE(controllerMap, json)
/*****************************************************************/
/*****************************************************************/

#define MAKE_ENUM(NAME, SUFFIX) NAME##_##SUFFIX,
#define MAKE_STRINGS(NAME, SUFFIX) #NAME "." #SUFFIX,

const char* const imageResourceStrings[] = {
    IMAGE_RESOURCE_ENUM(MAKE_STRINGS)
};
const char* const soundResourceStrings[] = {
	SOUND_RESOURCE_ENUM(MAKE_STRINGS)
};
const char* const musicPathResourceStrings[] = {
	MUSICPATH_RESOURCE_ENUM(MAKE_STRINGS)
};
const char* const fontResourceStrings[] = {
	FONT_RESOURCE_ENUM(MAKE_STRINGS)
};
const char* const shaderResourceStrings[] = {
	SHADER_RESOURCE_ENUM(MAKE_STRINGS)
};
const char* const jsonResourceStrings[] = {
	JSON_RESOURCE_ENUM(MAKE_STRINGS)
};

class ResourceManager {
public:
	enum ImageResources{ IMAGE_RESOURCE_ENUM(MAKE_ENUM)	};
	enum SoundResources{ SOUND_RESOURCE_ENUM(MAKE_ENUM)	};
	enum MusicPathResources{ MUSICPATH_RESOURCE_ENUM(MAKE_ENUM)	};
	enum FontResources{ FONT_RESOURCE_ENUM(MAKE_ENUM)	};
	enum ShaderResources{ SHADER_RESOURCE_ENUM(MAKE_ENUM)	};
	enum JSONResources{ JSON_RESOURCE_ENUM(MAKE_ENUM)	};

private:
	int imageResourceCount;
	std::map <int, sf::Image*> mImageMap;
	int soundResourceCount;
	std::map <int, sf::SoundBuffer*> mSoundMap;
	int musicPathResourceCount;
	std::map <int, std::string*> mMusicPathMap;
	int fontResourceCount;
	std::map <int, sf::Font*> mFontMap;
	int shaderResourceCount;
	std::map <int, std::string*> mShaderMap;
	int jsonResourceCount;
	std::map <int, rapidjson::Document*> mJsonMap;

public:
	ResourceManager();
	virtual ~ResourceManager();

	sf::Image* getImage(int texEnum);
	sf::SoundBuffer* getSoundBuffer(int soundBufferEnum);
	std::string* getMusicPath(int musicPathEnum);
	sf::Font* getFont(int fontEnum);
	std::string* getShader(int shaderEnum);
	rapidjson::Document* getDocument(int jsonEnum);
};

#endif /* RESOURCEMANAGER_HPP_ */
