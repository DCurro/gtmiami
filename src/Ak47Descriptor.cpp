#include "Ak47Descriptor.hpp"

Ak47Descriptor::Ak47Descriptor(int uniqueReferenceId, int floor, float x, float y, float angle)
: PooledPlayEntityDescriptor(uniqueReferenceId, floor, x, y, angle)
{
    this->mPOOLEDPLAYENTITY_TYPE = POOLEDPLAYENTITY_TYPE_AK47;
}

Ak47Descriptor::~Ak47Descriptor() {
    
}

rapidjson::Value* Ak47Descriptor::descriptorJsonObjectValue(rapidjson::Document* jsonDoc) {
    rapidjson::Value* value = new rapidjson::Value(rapidjson::kObjectType);
    
    rapidjson::Value typeValue;
    char buffer[100];
    int len = sprintf(buffer, "%s", this->mPOOLEDPLAYENTITY_TYPE.c_str());
    typeValue.SetString(buffer, len, jsonDoc->GetAllocator());
    
    value->AddMember("uniqueReferenceId", this->getUniqueReferenceId(), jsonDoc->GetAllocator());
    value->AddMember("type", typeValue, jsonDoc->GetAllocator());
    value->AddMember("floor", this->getFloor(), jsonDoc->GetAllocator());
    value->AddMember("x", this->getX(), jsonDoc->GetAllocator());
    value->AddMember("y", this->getY(), jsonDoc->GetAllocator());
    value->AddMember("angle", this->getAngle(), jsonDoc->GetAllocator());
    
    return value;
}