#ifndef ZombieDescriptor_HPP_
#define ZombieDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class ZombieDescriptor : public PooledPlayEntityDescriptor {
public:
    ZombieDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~ZombieDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif
