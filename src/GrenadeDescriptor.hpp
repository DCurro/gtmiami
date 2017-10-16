#ifndef GrenadeDescriptor_HPP_
#define GrenadeDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class GrenadeDescriptor : public PooledPlayEntityDescriptor {
public:
    GrenadeDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~GrenadeDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif