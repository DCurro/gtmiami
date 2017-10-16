#ifndef RocketLauncherDescriptor_HPP_
#define RocketLauncherDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class RocketLauncherDescriptor : public PooledPlayEntityDescriptor {
public:
    RocketLauncherDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~RocketLauncherDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif