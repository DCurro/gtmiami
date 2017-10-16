#ifndef ACTIONTIMER_HPP_
#define ACTIONTIMER_HPP_

#include <functional>

#include "common.hpp"

class ActionTimer {
protected:
	float mAccTime;

	GETTER(float, TimeToActive);
	GETTER(std::function<void()>, CallBackFunction);
	GETTER(int, RepeatTimes);

public:
	ActionTimer(float timeToActive, std::function<void()> callBack, int repeatTimes=1);
	virtual ~ActionTimer();

	virtual void update(float timeStep);
};

#endif /* ACTIONTIMER_HPP_ */
