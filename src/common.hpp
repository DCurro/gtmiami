#ifndef COMMON_HPP_
#define COMMON_HPP_

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>

#include <Box2D/Box2D.h>
#include <SFML/System.hpp>

#define PI_CONSTANT 3.14159265358979323846

#define RADIANS_TO_DEGREES (180.0f/PI_CONSTANT)
#define DEGREES_TO_RADIANS (PI_CONSTANT/180.0f)

#define AXIS_DEADZONE_RADIUS 20

#define ACCESSOR(type, var) \
protected: \
   type m##var; \
public: \
   virtual type get##var() \
   {\
      return m##var; \
   }\
   virtual void set##var(type val) \
   {\
      m##var = val; \
   }

#define GETTER(type, var) \
protected: \
   type m##var; \
public: \
   virtual type get##var() \
   {\
      return m##var; \
   }

#define GETTER_AND_DEFAULT_NULL(type, var) \
protected: \
   type m##var = 0; \
public: \
   virtual type get##var() \
   {\
      return m##var; \
   }

#define SETTER(type, var) \
protected: \
   type m##var; \
public: \
   virtual void set##var(type val) \
   {\
      m##var = val; \
   }

#define UNUSED(x) (void)x;

#define B2_TO_PIXEL_SCALE 50.0f
float worldToScreen(float worldSize);
float screenToWorld(float screenScalar);
float angleInDegreesFromVector(b2Vec2 vector);
float angleInRadiansFromVector(b2Vec2 vector);
float validJoyAxisXPos(float xpos, float ypos);
float validJoyAxisYPos(float xpos, float ypos);

float vecMagnitude(float x, float y);

sf::Vector2f getScaleDifference(sf::Vector2u fromDimensions, sf::Vector2u toDimensions);
sf::Vector2f getScaleDifferenceMaintainPerspective(sf::Vector2u fromDimensions, sf::Vector2u toDimensions);
sf::Vector2f getScaledLocationMaintainPerspective(sf::Vector2u fromDimensions, sf::Vector2u toDimensions);

void test_validJoyAxisYPos();

int mod(int a, int b);

float minX(sf::Vector2f firstVec, sf::Vector2f secondVec);
float maxX(sf::Vector2f firstVec, sf::Vector2f secondVec);
float minY(sf::Vector2f firstVec, sf::Vector2f secondVec);
float maxY(sf::Vector2f firstVec, sf::Vector2f secondVec);

std::string intToString( int i );

#endif /* COMMON_HPP_ */
