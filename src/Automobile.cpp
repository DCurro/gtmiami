#include "Automobile.hpp"
#include "GameContext.hpp"

#include <sstream>
#include <set>
#include <assert.h>

#include "PlayEntity.hpp"
#include "PlayContext.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"
#include "EntityManager.hpp"
#include "DriverBody.hpp"

Automobile::Automobile() : PooledPlayEntity() {
	mDriver = NULL;
	mGasPower = 0.0f;
	mBreakPower = 0.0f;
	mTurnPower = 0.0f;

	mLastSpeed = 0.0f;

	mIsStopped = true;
	mCanReverse = true;
	mStoppedTimer = 0.0f;

	mIsParked = true;

	this->clearDriverInput();

	mPriorityOrderedDoorSensorFixtures = NULL;
	mDoorExitLocationObstructionMultiSetList = NULL;

	mCurrentAnimaton = NULL;
}

Automobile::~Automobile() {
	delete mPriorityOrderedDoorSensorFixtures;
	delete mDoorExitLocationObstructionMultiSetList;
}

void Automobile::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
	PooledPlayEntity::setup(playContext, persistentIdNumber, floor);

	mPriorityOrderedDoorSensorFixtures = new std::vector<b2Fixture*>();
	mDoorExitLocationObstructionMultiSetList = new std::vector<std::multiset<PlayEntity*>*>();
}

void Automobile::prepareForReuse() {
	PooledPlayEntity::prepareForReuse();

	if(this->mStaticBody) {
		this->getPlayContext()->getWorld()->DestroyBody(this->mStaticBody);
	}

	if(mPriorityOrderedDoorSensorFixtures) {
		delete mPriorityOrderedDoorSensorFixtures;
	}
	mPriorityOrderedDoorSensorFixtures = NULL;

	if(mDoorExitLocationObstructionMultiSetList) {
		delete mDoorExitLocationObstructionMultiSetList;
	}
	mDoorExitLocationObstructionMultiSetList = NULL;

	mDriver = NULL;
	mGasPower = 0.0f;
	mBreakPower = 0.0f;
	mTurnPower = 0.0f;

	mLastSpeed = 0.0f;

	mIsStopped = true;
	mCanReverse = true;
	mStoppedTimer = 0.0f;

	mIsParked = true;

	this->clearDriverInput();

	mCurrentAnimaton = NULL;
}

void Automobile::update(float timeStep){
	mCurrentAnimaton = NULL;

	if(this->idleAnimation() != NULL) {
		mCurrentAnimaton = this->idleAnimation();
	}

	if(mCurrentAnimaton) {
		mCurrentAnimaton->update(timeStep);
		mSprite.setTexture(*mCurrentAnimaton->getCurrentFrame(), true);
	}

	this->updateFriction();
	this->getStaticBody()->SetTransform(this->getBody()->GetPosition(),this->getBody()->GetAngle());

	PooledPlayEntity* entity = dynamic_cast<PooledPlayEntity*>(mDriver);
	if(isValid(entity)){
		entity->getBody()->SetTransform(this->getBody()->GetPosition(),this->getBody()->GetAngle());
		mDriver->drive();
		this->updateDrive(timeStep);
		this->updateTurn(timeStep);

		this->clearDriverInput();
	}

	PooledPlayEntity::update(timeStep);
}

void Automobile::addDoorSensorFixtureAsNextMostPriority(b2Fixture* doorSensorFixture) {
	if(doorSensorFixture){
		mPriorityOrderedDoorSensorFixtures->push_back(doorSensorFixture);
		mDoorExitLocationObstructionMultiSetList->push_back(new std::multiset<PlayEntity*>());
	}
}

b2Vec2 Automobile::getCircleFixtureWorldCentre(b2Body* body, b2Fixture* circleShapedFixture){
	b2Transform transform = body->GetTransform();

	b2CircleShape* circleShape = static_cast<b2CircleShape*>(circleShapedFixture->GetShape());
	assert(circleShape != NULL); //assert that it must be a circle shape

	b2Vec2 driverDoorVec = circleShape->m_p;

	return b2Mul(transform, driverDoorVec);
}

float Automobile::angularImpulse() {
	float angularImpulseValue = this->angularImpulseWhileInDrive();
	if(mIsParked) {
		angularImpulseValue = this->angularImpulseWhileParked();
	}

	return angularImpulseValue;
}

//TODO: make sure caller deletes b2Vec when they are done with it.
b2Vec2* Automobile::preferredExitLocation() {
	b2Vec2* exitLocation = NULL;

	for(unsigned long i=0; i<mPriorityOrderedDoorSensorFixtures->size(); i++){
		b2Fixture* fixture = mPriorityOrderedDoorSensorFixtures->at(i);
		std::multiset<PlayEntity*>* obstructionSet = mDoorExitLocationObstructionMultiSetList->at(i);

		if(obstructionSet->empty()) {
			exitLocation = new b2Vec2();
			b2Vec2 exitLoc = getCircleFixtureWorldCentre(this->getBody(), fixture);
			exitLocation->x = exitLoc.x;
			exitLocation->y = exitLoc.y;

			break;
		}
	}

	return exitLocation;
}

std::vector<b2Vec2> Automobile::getDoorPositions() {
	std::vector<b2Vec2> returnList;

	for(unsigned long i=0; i<mPriorityOrderedDoorSensorFixtures->size(); i++){
		b2Fixture* fixture = mPriorityOrderedDoorSensorFixtures->at(i);

		b2Vec2 exitLoc = getCircleFixtureWorldCentre(this->getBody(), fixture);
		returnList.push_back(exitLoc);
	}

	return returnList;
}

float Automobile::dragAndTireFrictionCoefficient() {
	float dragAndTireFriction = this->dragAndTireFrictionWhileInDriveCoefficient();
	if(mIsParked) {
		dragAndTireFriction = this->dragAndTireFrictionWhileParkedCoefficient();
	}

	return dragAndTireFriction;
}

void Automobile::applyGas(float gasPower) {
	this->mGasPower = gasPower;
}

void Automobile::applyBreak() {
	this->mBreakPower = 100.0f;
}

void Automobile::turnWheel(float turnPower) {
	this->mTurnPower = turnPower;
}

void Automobile::dropOffDriver() {
	PooledPlayEntity* entity = dynamic_cast<PooledPlayEntity*>(mDriver);
	if(isValid(entity)){
		b2Vec2* exitLocation = this->preferredExitLocation();
		if(exitLocation) {
			entity->getBody()->SetTransform(*exitLocation, 0.0f);

			mDriver->stopDrivingAutomobile();
			mDriver = NULL;

			delete exitLocation;
		}
	}
}

float Automobile::updateDrive(float timeStep){
	b2Vec2 currentForwardNormal = mBody->GetWorldVector( b2Vec2(1,0) );
	b2Vec2 localVelocityVector = mBody->GetLocalVector(mBody->GetLinearVelocity());
	float currentSpeed = localVelocityVector.x;

	float m_maxDriveForce = this->maxForwardForce();
	float m_maxBreakForce = this->maxBreakingForce();

	float finalForwardForce = 0.0f;
	float force = 0.0f;
	float normalizedGasPower = mGasPower/100.0f;
	float normalizedBreakPower = mBreakPower/100.0f;

	mIsStopped = false;

	if(currentSpeed > 0.0f){//	moving forward
		if(mBreakPower >= mGasPower) { //applying breaks
			force = -m_maxBreakForce*normalizedBreakPower;
		} else { //applying gas
			force = m_maxDriveForce*normalizedGasPower;
		}
		mCanReverse = false;
	} else if(currentSpeed < 0.0f){//	moving backwards
		if(mBreakPower >= mGasPower) { //applying gas
			force = -m_maxDriveForce*normalizedBreakPower;
		} else { //applying break
			force = m_maxBreakForce*normalizedGasPower;
		}
		mCanReverse = false;
	} else {//	standing still
		mIsStopped = true;
		if(mBreakPower >= mGasPower) { //applying breaks
			if(mCanReverse){
				force = -m_maxDriveForce*normalizedBreakPower;
			}
		} else { //applying gas
			force = m_maxDriveForce*normalizedGasPower;
		}
	}

	if(mIsStopped) {
		mIsParked = true;
		mStoppedTimer += timeStep;
		if(mStoppedTimer > 0.5f){
			mCanReverse = true;
			mStoppedTimer = 0.0f;
		}
	} else {
		mIsParked = false;
	}

//	switching between forward and backwards motion
	if((currentSpeed > 0.0f && mLastSpeed < 0.0f) || (mLastSpeed > 0.0f && currentSpeed < 0.0f)) {
		b2Rot rotation = mBody->GetTransform().q;
		b2Vec2 localVelocityNoForward = b2Vec2(0,localVelocityVector.y);
		b2Vec2 finalVelocity = b2Mul(rotation, localVelocityNoForward);

		mBody->SetLinearVelocity(finalVelocity);

		finalForwardForce = 0.0f;
	} else { //maintaining direction
		b2Vec2 forceApplied = force*currentForwardNormal;
		if(currentSpeed>0 && force>0 && currentSpeed > this->maxForwardVelocity()){
			forceApplied = b2Vec2(0,0);
		} else if(currentSpeed<0 && force<0 && currentSpeed < -this->maxReverseVelocity()){
			forceApplied = b2Vec2(0,0);
		}

		mBody->ApplyForce(forceApplied, mBody->GetWorldCenter(), true);

		finalForwardForce = force;
	}

	mLastSpeed = currentSpeed;

	return finalForwardForce;
}

void Automobile::updateTurn(float timeStep){
	b2Vec2 localVelocityVector = mBody->GetLocalVector(mBody->GetLinearVelocity());
	float currentSpeed = localVelocityVector.x;

	float torqueToApply = currentSpeed/timeStep;

	float normalizedTurnPower = mTurnPower/100.0f;

	mBody->ApplyTorque(this->turnCoefficient()*normalizedTurnPower*torqueToApply*50.0f, true);
}

void Automobile::clearDriverInput() {
	mGasPower = 0.0f;
	mTurnPower = 0.0f;
	mBreakPower = 0.0f;
}

bool Automobile::setDriver(DriverBody* driver){
	if(!driver) {
		return false;
	}

	PooledPlayEntity* entity = dynamic_cast<PooledPlayEntity*>(driver);
	registerPooledPlayEntity(entity);
	if(!entity) {
		return false;
	}

	mDriver = driver;
	mDriver->startDrivingAutomobile(this);
	entity->registerPooledPlayEntity(this);
	entity->clearPhysicalMotion();

	return true;
}

bool Automobile::hasDriver() {
	PooledPlayEntity* entity = dynamic_cast<PooledPlayEntity*>(mDriver);
	return isValid(entity);
}

b2Vec2 Automobile::getForwardVelocity() {
	b2Vec2 currentRightNormal = mBody->GetWorldVector( b2Vec2(1,0) );
	return b2Dot( currentRightNormal, mBody->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 Automobile::getLateralVelocity() {
	b2Vec2 currentRightNormal = mBody->GetWorldVector( b2Vec2(0,1) );
	return b2Dot( currentRightNormal, mBody->GetLinearVelocity() ) * currentRightNormal;
}

void Automobile::updateFriction() {
	b2Vec2 forwardImpulse = mBody->GetMass() * -getLateralVelocity();
	float maxLateralImpulse = this->maxImpulseBeforeDrifting();
	if ( forwardImpulse.Length() > maxLateralImpulse ) {
	      forwardImpulse *= maxLateralImpulse / forwardImpulse.Length();
	}
	mBody->ApplyLinearImpulse( forwardImpulse, mBody->GetWorldCenter(), true );
	mBody->ApplyAngularImpulse( this->angularImpulse() * mBody->GetInertia() * -mBody->GetAngularVelocity(), true );

	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -this->dragAndTireFrictionCoefficient()*currentForwardSpeed;

	mBody->ApplyForce( dragForceMagnitude * currentForwardNormal, mBody->GetWorldCenter(), true );
}

// PlayEntity

Animation* Automobile::newBulletStruckAnimation() {
	sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::greyBulletStrikeAnimation_png);;
	return new Animation(false, 12, image);
}

void Automobile::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void Automobile::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}
