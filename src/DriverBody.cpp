#include "DriverBody.hpp"

void DriverBody::startDrivingAutomobile(Automobile* automobile) {
	if(automobile){
		this->willBeginDriving();
		this->mAutomobileDriving = automobile;
	}
}

void DriverBody::stopDrivingAutomobile(){
	this->willEndDriving();
	this->mAutomobileDriving = NULL;
}
