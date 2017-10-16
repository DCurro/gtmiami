#ifndef DRIVERBODY_HPP_
#define DRIVERBODY_HPP_

#include "Automobile.hpp"

class DriverBody {
protected:
	GETTER_AND_DEFAULT_NULL(Automobile*, AutomobileDriving);

public:
	virtual ~DriverBody() {
	}

	virtual void drive() = 0;
	virtual void willBeginDriving() = 0;
	virtual void willEndDriving() = 0;

	virtual void startDrivingAutomobile(Automobile* automobile);
	virtual void stopDrivingAutomobile();

	virtual void requestDropOffDriver() = 0;
	virtual void requestApplyBreak() = 0;
	virtual void requestApplyGas(float gasPower) = 0;
	virtual void requestTurnWheel(float turnPower) = 0;
};

#endif /* DRIVERBODY_HPP_ */
