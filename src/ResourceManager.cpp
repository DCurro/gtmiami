#include "ResourceManager.hpp"

#include <fstream>
#include <iostream>

#include "extlibs/rapidjson/prettywriter.h"	// for stringify JSON
#include "extlibs/rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

#include "OS.hpp"

std::string stringConcat(std::string string, std::string concat){
	return string + concat;
}

ResourceManager::ResourceManager() {
	std::cout << "RESOURCE MANAGER CONSTRUCTOR" << std::endl;

	std::string currentDirectory = OS::getPwd();
	std::string resourcePath = currentDirectory + "resources/";

	imageResourceCount = sizeof(imageResourceStrings)/sizeof(imageResourceStrings[0]);
	for(int i=0; i<imageResourceCount; i++){
		mImageMap[i] = NULL;
	}

	soundResourceCount = sizeof(soundResourceStrings)/sizeof(soundResourceStrings[0]);
	for(int i=0; i<soundResourceCount; i++){
		mSoundMap[i] = NULL;
	}

	musicPathResourceCount = sizeof(musicPathResourceStrings)/sizeof(musicPathResourceStrings[0]);
	std::cout << "Loading " << musicPathResourceCount << " music paths:" << std::endl;
	for(int i=0; i<musicPathResourceCount; i++){
		std::cout << "\t" << musicPathResourceStrings[i] << std::endl;
		std::string musicFilePath = resourcePath+"music/"+musicPathResourceStrings[i];
		std::string* resourcePath = new std::string(musicFilePath);
		mMusicPathMap[i] = resourcePath;
	}

	fontResourceCount = sizeof(fontResourceStrings)/sizeof(fontResourceStrings[0]);
	std::cout << "Loading " << fontResourceCount << " font resources:" << std::endl;
	for(int i=0; i<fontResourceCount; i++){
		std::cout << "\t" << fontResourceStrings[i] << std::endl;
		sf::Font* font = new sf::Font();
		font->loadFromFile(resourcePath+fontResourceStrings[i]);
		mFontMap[i] = font;
	}

	shaderResourceCount = sizeof(shaderResourceStrings)/sizeof(shaderResourceStrings[0]);
	std::cout << "Loading " << shaderResourceCount << " shader resources:" << std::endl;
	for(int i=0; i<shaderResourceCount; i++){
		std::cout << "\t" << shaderResourceStrings[i] << std::endl;
		//http://insanecoding.blogspot.ca/2011/11/how-to-read-in-file-in-c.html
		std::ifstream in(resourcePath+shaderResourceStrings[i], std::ios::in | std::ios::binary);
		std::string* shaderString = new std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		mShaderMap[i] = shaderString;
	}

	jsonResourceCount = sizeof(jsonResourceStrings)/sizeof(jsonResourceStrings[0]);
	std::cout << "Loading " << jsonResourceCount << " json resources:" << std::endl;
	for(int i=0; i<jsonResourceCount; i++){
		std::cout << "\t" << jsonResourceStrings[i] << std::endl;
//		http://insanecoding.blogspot.ca/2011/11/how-to-read-in-file-in-c.html
		std::ifstream in(resourcePath+jsonResourceStrings[i], std::ios::in | std::ios::binary);
		std::string* jsonString = new std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		rapidjson::Document* jsonDoc = new rapidjson::Document();
		jsonDoc->Parse(jsonString->c_str());
		mJsonMap[i] = jsonDoc;
	}

	std::cout << std::endl;
}

ResourceManager::~ResourceManager() {
	//TODO: clean up memory when done
}

sf::Image* ResourceManager::getImage(int texEnum){
	if(mImageMap[texEnum] == NULL){
		std::string currentDirectory = OS::getPwd();
        std::cout << "pwd is " << currentDirectory << std::endl;
		std::string resourcePath = currentDirectory + "resources/images/";

		std::cout << "Loading " << imageResourceStrings[texEnum];
		sf::Image* image = new sf::Image();
		if( image->loadFromFile(resourcePath+imageResourceStrings[texEnum]) ) {
			mImageMap[texEnum] = image;
			std::cout << " succeeded." << std::endl;
		} else {
			std::cout << " failed." << std::endl;
		}
	}

	return mImageMap[texEnum];
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(int soundBufferEnum){
	if(mSoundMap[soundBufferEnum] == NULL) {
		std::string currentDirectory = OS::getPwd();
		std::string resourcePath = currentDirectory + "resources/soundEffects/";

		sf::SoundBuffer* soundBuff = new sf::SoundBuffer();

		if(soundBuff->loadFromFile(resourcePath+soundResourceStrings[soundBufferEnum])){
			std::cout << "Loading " << soundResourceStrings[soundBufferEnum];
			mSoundMap[soundBufferEnum] = soundBuff;
			std::cout << " succeeded" << std::endl;
		} else {
			std::cout << "Loading " << soundResourceStrings[soundBufferEnum];
			std::cout << " failed" << std::endl;
		}

		mSoundMap[soundBufferEnum] = soundBuff;
	}

	return mSoundMap[soundBufferEnum];
}

std::string* ResourceManager::getMusicPath(int musicPathEnum){
	return mMusicPathMap[musicPathEnum];
}

sf::Font* ResourceManager::getFont(int fontEnum){
	return mFontMap[fontEnum];
}

std::string* ResourceManager::getShader(int shaderEnum){
	return mShaderMap[shaderEnum];
}

rapidjson::Document* ResourceManager::getDocument(int jsonEnum){
	return mJsonMap[jsonEnum];
}

