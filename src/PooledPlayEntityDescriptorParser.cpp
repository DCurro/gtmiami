#include "PooledPlayEntityDescriptorParser.hpp"

#include "PooledPlayEntityDescriptor.hpp"
#include "BunnyDescriptor.hpp"
#include "TurretDescriptor.hpp"
#include "ZombieDescriptor.hpp"
#include "SpiritDescriptor.hpp"
#include "CarDescriptor.hpp"
#include "TowTruckDescriptor.hpp"
#include "RevolverDescriptor.hpp"
#include "Ak47Descriptor.hpp"
#include "RocketLauncherDescriptor.hpp"
#include "GrenadeDescriptor.hpp"

PooledPlayEntityDescriptorParser::PooledPlayEntityDescriptorParser() {
    
}

PooledPlayEntityDescriptorParser::~PooledPlayEntityDescriptorParser() {
    
}

PooledPlayEntityDescriptor* PooledPlayEntityDescriptorParser::descriptorForJsonDescriptorValue(rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> >::ConstValueIterator constValueIterator)
{
    PooledPlayEntityDescriptor* returnDescriptor = NULL;
    
    std::string type(constValueIterator->FindMember("type")->value.GetString());
    
    if (type.compare(POOLEDPLAYENTITY_TYPE_BUNNY) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();

        BunnyDescriptor *descriptor = new BunnyDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
    
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_TURRET) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        TurretDescriptor *descriptor = new TurretDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
    
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_ZOMBIE) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        ZombieDescriptor *descriptor = new ZombieDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_SPIRIT) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        SpiritDescriptor *descriptor = new SpiritDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_CAR) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        CarDescriptor *descriptor = new CarDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_TOWTRUCK) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        TowTruckDescriptor *descriptor = new TowTruckDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_REVOLVER) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        RevolverDescriptor *descriptor = new RevolverDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_AK47) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        Ak47Descriptor *descriptor = new Ak47Descriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_ROCKETLAUNCHER) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        RocketLauncherDescriptor *descriptor = new RocketLauncherDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else if (type.compare(POOLEDPLAYENTITY_TYPE_GRENADE) == 0) {
        int uniqueReferenceId = constValueIterator->FindMember("uniqueReferenceId")->value.GetInt();
        int floor = constValueIterator->FindMember("floor")->value.GetInt();
        float x = constValueIterator->FindMember("x")->value.GetDouble();
        float y = constValueIterator->FindMember("y")->value.GetDouble();
        float angle = constValueIterator->FindMember("angle")->value.GetDouble();
        
        GrenadeDescriptor *descriptor = new GrenadeDescriptor(uniqueReferenceId, floor, x, y, angle);
        returnDescriptor = descriptor;
        
    } else {
        assert(returnDescriptor!=NULL);
    }
    
    return returnDescriptor;
}