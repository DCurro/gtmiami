#ifndef CarDescriptor_HPP_
#define CarDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class CarDescriptor : public PooledPlayEntityDescriptor {
public:
    CarDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~CarDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif