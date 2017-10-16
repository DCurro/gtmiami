#include "common.hpp"

#include <assert.h>
#include <iostream>

float worldToScreen(float worldSize){
	return B2_TO_PIXEL_SCALE*worldSize;
}
float screenToWorld(float screenScalar){
	return (1.0f/B2_TO_PIXEL_SCALE)*screenScalar;
}

float angleInDegreesFromVector(b2Vec2 vector) {
	float radangle = atan(vector.y/vector.x);
	float angle = radangle * (180.0f/PI_CONSTANT) + 90.0f;
	if(vector.x<0){ angle = angle+180; }
	angle = (angle-90);
	angle = (int)angle%360;
	if(angle < 0){ angle = 360-fabs(angle); }

	return angle;
}

float angleInRadiansFromVector(b2Vec2 vector) {
	return angleInDegreesFromVector(vector) * (PI_CONSTANT/180.0f);
}

float validJoyAxisXPos(float xpos, float ypos){
	if(vecMagnitude(xpos,ypos)>AXIS_DEADZONE_RADIUS){
		return xpos;
	} else {
		return 0.0f;
	}
}

float validJoyAxisYPos(float xpos, float ypos){
	if(vecMagnitude(xpos,ypos)>AXIS_DEADZONE_RADIUS){
		return ypos;
	} else {
		return 0.0f;
	}
}


float vecMagnitude(float x, float y){
	return std::sqrt(x*x + y*y);
}

void test_validJoyAxisYPos(){
	float x = validJoyAxisXPos(100.0f, 0.0f);
	assert(x == 100.0f);
	x = validJoyAxisXPos(25.0f, 0.0f);
	assert(x == 25.0f);
	float y = validJoyAxisYPos(0.0f, 21.0f);
	assert(y == 21.0f);
	y = validJoyAxisYPos(0.0f, 100.0f);
	assert(y == 100.0f);

	y = validJoyAxisYPos(21.0f, 21.0f);
	assert(y == 21.0f);
	y = validJoyAxisYPos(5.0f, 5.0f);
	assert(y == 0.0f);
}

sf::Vector2f getScaleDifference(sf::Vector2u fromDimensions, sf::Vector2u toDimensions){
	return sf::Vector2f((float)toDimensions.x/(float)fromDimensions.x, (float)toDimensions.y/(float)fromDimensions.y);
}
sf::Vector2f getScaleDifferenceMaintainPerspective(sf::Vector2u fromDimensions, sf::Vector2u toDimensions){
	float xScale = (float)toDimensions.x/(float)fromDimensions.x;
	float yScale = (float)toDimensions.y/(float)fromDimensions.y;
	if(fromDimensions.y*xScale < toDimensions.y){
		return sf::Vector2f(xScale,xScale);
	} else
	if(fromDimensions.x*yScale < toDimensions.x){
		return sf::Vector2f(yScale,yScale);
	} else {
		return sf::Vector2f(1.0f,1.0f); //strange case... don't scale
	}
}

sf::Vector2f getScaledLocationMaintainPerspective(sf::Vector2u fromDimensions, sf::Vector2u toDimensions){
	float xScale = (float)toDimensions.x/(float)fromDimensions.x;
	float yScale = (float)toDimensions.y/(float)fromDimensions.y;
	float scale = 0.0f;
	bool scaledX = true;
	if(fromDimensions.y*xScale < toDimensions.y){
		scale = xScale;
		scaledX = true;
	} else
	if(fromDimensions.x*yScale < toDimensions.x){
		scale = yScale;
		scaledX = false;
	} else {
		scale = 1.0f; //strange case... don't reposition
		scaledX = false;
		return sf::Vector2f(0.0f,0.0f);
	}

	if(scaledX){
		return sf::Vector2f( 0.0f,
				             (toDimensions.y-scale*fromDimensions.y)/2.0f);
	} else {
		return sf::Vector2f( (toDimensions.x-scale*fromDimensions.x)/2.0f,
						     0.0f);
	}
}

/* mod
 *
 * Used like a%b.
 * This is different for the remainder operator %, as it gives the appropriate modulo of negative numbers.
 */
int mod(int a, int b){
	int r = a % b;
	return r < 0 ? r + b : r;
}

float minX(sf::Vector2f firstVec, sf::Vector2f secondVec) {
	return (firstVec.x <= secondVec.x) ? firstVec.x: secondVec.x;
}
float maxX(sf::Vector2f firstVec, sf::Vector2f secondVec) {
	return (firstVec.x >= secondVec.x) ? firstVec.x: secondVec.x;
}
float minY(sf::Vector2f firstVec, sf::Vector2f secondVec) {
	return (firstVec.y <= secondVec.y) ? firstVec.y: secondVec.y;
}
float maxY(sf::Vector2f firstVec, sf::Vector2f secondVec) {
	return (firstVec.y >= secondVec.y) ? firstVec.y: secondVec.y;
}

std::string intToString(int i){
    std::stringstream ss;
    ss<<i;
    return ss.str();
}


