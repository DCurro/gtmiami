#ifndef TowTruckDescriptor_HPP_
#define TowTruckDescriptor_HPP_

#include "PooledPlayEntityDescriptor.hpp"
#include "Common.hpp"

class TowTruckDescriptor : public PooledPlayEntityDescriptor {
public:
    TowTruckDescriptor(int uniqueReferenceId, int floor, float x, float y, float angle);
    virtual ~TowTruckDescriptor();
    
    virtual rapidjson::Value* descriptorJsonObjectValue(rapidjson::Document* jsonDoc) override;
};

#endif