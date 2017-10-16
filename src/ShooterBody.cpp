#include "ShooterBody.hpp"

ShooterBody::ShooterBody() {
	mHeldGun = NULL;
}

ShooterBody::~ShooterBody() {}

void ShooterBody::dropGun() {
	if(mHeldGun) {
//		mHeldGun->putBackDown();
		mHeldGun = NULL;
	}
}

void ShooterBody::holdGun(Gun* gun) {
	if(!isArmed() && gun) {
		mHeldGun = gun;
	}
}

bool ShooterBody::isArmed() {
	if(mHeldGun) {
		return true;
	}
	return false;
}

void ShooterBody::pullTheTrigger() {
	if(isArmed()) {
//		mHeldGun->fireGun();
	}
}

