/*
 * GameTime.cpp
 *
 *  Created on: Nov 1, 2014
 *      Author: Evil Overlord
 */

#include "GameTime.hpp"

GameTime::GameTime(float timeStepPerMinute) {
	totalTimeSinceStart = 0.0f;
	timePerMinute = timeStepPerMinute;
}

GameTime::~GameTime() {
}

int GameTime::getSecond() {
	return (totalTimeSinceStart/timePerMinute)*60 - (getMinute()*60) - (getHour()*60*60) - (getDay()*24*60*60);
}

int GameTime::getMinute() {
	return (totalTimeSinceStart/timePerMinute) - (getHour()*60) - (getDay()*24*60);
}

int GameTime::getHour() {
	return (totalTimeSinceStart/(timePerMinute*60)) - (getDay()*24);
}

int GameTime::getDay() {
	int dayOffset = totalTimeSinceStart >= 0.0f ? 0 : -1;
	return dayOffset + totalTimeSinceStart / (timePerMinute*24*60);
}

float GameTime::getTimeStepSum() {
	return totalTimeSinceStart;
}

void GameTime::setTime( int day, int hour, int minute, int second ) {
	totalTimeSinceStart = (day*timePerMinute*24*60) + (hour*timePerMinute*60) + (minute*timePerMinute) + ((second*timePerMinute)/60);
}

void GameTime::setTime(float totalTimeStep) {
	totalTimeSinceStart = totalTimeStep;
}

void GameTime::update(float timeStep) {
	totalTimeSinceStart += timeStep;
}
