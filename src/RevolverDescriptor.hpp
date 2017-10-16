#ifndef RevolverDescriptor_HPP_
#define RevolverDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class RevolverDescriptor : public PooledPlayEntityDescriptor {
public:
    RevolverDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~RevolverDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif