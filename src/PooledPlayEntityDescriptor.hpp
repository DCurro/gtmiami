#ifndef PooledPlayEntityDescriptor_HPP_
#define PooledPlayEntityDescriptor_HPP_

#include "GameObject.hpp"
#include "Common.hpp"

#include "extlibs/rapidjson/document.h"

#define POOLEDPLAYENTITY_TYPE_HERO              "Hero"
#define POOLEDPLAYENTITY_TYPE_TURRET            "Turret"
#define POOLEDPLAYENTITY_TYPE_BUNNY             "Bunny"
#define POOLEDPLAYENTITY_TYPE_ZOMBIE            "Zombie"
#define POOLEDPLAYENTITY_TYPE_SPIRIT            "Spirit"
#define POOLEDPLAYENTITY_TYPE_CAR               "Car"
#define POOLEDPLAYENTITY_TYPE_TOWTRUCK          "TowTruck"
#define POOLEDPLAYENTITY_TYPE_REVOLVER          "Revolver"
#define POOLEDPLAYENTITY_TYPE_AK47              "Ak47"
#define POOLEDPLAYENTITY_TYPE_ROCKETLAUNCHER    "RocketLauncher"
#define POOLEDPLAYENTITY_TYPE_GRENADE           "Grenade"

class PooledPlayEntityDescriptor : public GameObject {
public:
    GETTER(int, UniqueReferenceId);
    GETTER(std::string, POOLEDPLAYENTITY_TYPE);
    GETTER(int, Floor);
    GETTER(float, X);
    GETTER(float, Y);
    GETTER(float, Angle);
    
    PooledPlayEntityDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~PooledPlayEntityDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) = 0;
};

#endif
