#ifndef Ak47Descriptor_HPP_
#define Ak47Descriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class Ak47Descriptor : public PooledPlayEntityDescriptor {
public:
    Ak47Descriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~Ak47Descriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif