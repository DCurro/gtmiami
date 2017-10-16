/*
 * GameTime.hpp
 *
 *  Created on: Nov 1, 2014
 *      Author: Evil Overlord
 */

#ifndef GAMETIME_HPP_
#define GAMETIME_HPP_

class GameTime {
private:
	float totalTimeSinceStart;
	float timePerMinute;

public:
	GameTime(float timeStepPerMinute);
	virtual ~GameTime();

	int getSecond();
	int getMinute();
	int getHour();
	int getDay();
	float getTimeStepSum();

	void setTime(int day, int hour, int minute, int second);
	void setTime(float totalTimeStep);

	virtual void update(float timeStep);
};

#endif /* GAMETIME_HPP_ */
