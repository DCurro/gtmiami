#ifndef TurretDescriptor_HPP_
#define TurretDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class TurretDescriptor : public PooledPlayEntityDescriptor {
public:
    TurretDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~TurretDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif
