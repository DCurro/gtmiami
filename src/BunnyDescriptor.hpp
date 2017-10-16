#ifndef BunnyDescriptor_HPP_
#define BunnyDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class BunnyDescriptor : public PooledPlayEntityDescriptor {
public:
    BunnyDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~BunnyDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif
