#ifndef SpiritDescriptor_HPP_
#define SpiritDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class SpiritDescriptor : public PooledPlayEntityDescriptor {
public:
    SpiritDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~SpiritDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif