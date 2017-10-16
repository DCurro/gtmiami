#include "HeroMind.hpp"

#include "InputHandler.hpp"
#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "Camera.hpp"
#include "Animal.hpp"

#include "DriverBody.hpp"

HeroMind::HeroMind() {

}

HeroMind::~HeroMind() {

}

void HeroMind::setup(PlayContext* playContext, Animal* controllingAnimal) {
	Mind::setup(playContext, controllingAnimal);

	mDesiredLookangleInDegrees = 0.0f;
}

void HeroMind::update(float timeStep) {
	Mind::update(timeStep);

	InputHandler* inputHandler = mPlayContext->getGameContext()->getInputHandler();

	b2Vec2 motionVector(inputHandler->getLeftJoystickPosition().x, inputHandler->getLeftJoystickPosition().y);
	motionVector.Normalize();
	float power = std::max(motionVector.Length(), 1.0f);
	if(motionVector.x !=0 || motionVector.y !=0){
		mAnimal->applyMotion(motionVector, power);
	}

	b2Vec2 directionVector(inputHandler->getRightJoystickPosition().x, inputHandler->getRightJoystickPosition().y);
	if(directionVector.Length() != 0){
		mDesiredLookangleInDegrees = angleInDegreesFromVector(directionVector);
	}

	if(directionVector.Length() > 90.0f) {
		mAnimal->applyBeginAttack(mDesiredLookangleInDegrees);
	} else {
		mAnimal->applyEndAttack(mDesiredLookangleInDegrees);
	}

	if(inputHandler->button1WasTapped_consumeInput()) {
		mAnimal->pickUpClosestWeapon();
	}

	if(mAnimal->getIsDriving() == false && inputHandler->button4WasTapped_consumeInput()) {
		mAnimal->applyTryDriveClosestVehicle();
	}

	if(inputHandler->rightWasTapped_consumeInput()) {
		mAnimal->applySwitchWeapon();
	}

	mPlayContext->getCamera()->requestImmediateFocus(this->mAnimal);
}

void HeroMind::drive() {
	if(!isValid(mAnimal)){
		return;
	}

	DriverBody* driver = dynamic_cast<DriverBody*>(mAnimal);
	if(!driver) {
		return;
	}

	InputHandler* inputHandler = mPlayContext->getGameContext()->getInputHandler();

	if(inputHandler->button4WasTapped_consumeInput()) {
		driver->requestDropOffDriver();
		return;
	}

	if(inputHandler->getButton3()) {
		driver->requestApplyBreak();
	} else if(inputHandler->getR2Position() > 0){
		float gasPower = inputHandler->getR2Position();
		driver->requestApplyGas(gasPower);
	}

	float turnPower = inputHandler->getLeftJoystickPosition().x;
	if(turnPower != 0){
		std::cout << "turnPower" << std::endl;
		driver->requestTurnWheel(turnPower);
	}
}
