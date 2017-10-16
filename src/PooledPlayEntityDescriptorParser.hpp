#ifndef PooledPlayEntityDescriptorParser_HPP_
#define PooledPlayEntityDescriptorParser_HPP_

#include "extlibs/rapidjson/document.h"

#include "GameObject.hpp"
#include "Common.hpp"

class PooledPlayEntityDescriptor;


class PooledPlayEntityDescriptorParser : public GameObject {
public:
    PooledPlayEntityDescriptorParser();
    virtual ~PooledPlayEntityDescriptorParser();
    
    PooledPlayEntityDescriptor* descriptorForJsonDescriptorValue(rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> >::ConstValueIterator constValueIterator);
};

#endif
