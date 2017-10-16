#include "ActionTimer.hpp"

ActionTimer::ActionTimer(float timeToActive, std::function<void()> callBackFunction, int repeatTimes) {
	mAccTime = 0.0f;
	mTimeToActive = timeToActive;
	mCallBackFunction = callBackFunction;
	mRepeatTimes = repeatTimes;
}

ActionTimer::~ActionTimer() {
}


void ActionTimer::update(float timeStep){
	mAccTime += timeStep;

	while(mAccTime > mTimeToActive) {
		if(mRepeatTimes > 0){
			mRepeatTimes--;
			mAccTime -= mTimeToActive;
			mCallBackFunction();
		} else {
			break;
		}
	}
}
