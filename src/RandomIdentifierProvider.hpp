#ifndef RandomIdentifierProvider_HPP_
#define RandomIdentifierProvider_HPP_

#include "GameObject.hpp"
#include "Common.hpp"

#define NO_PERSISTENCE_ID 0

class RandomIdentifierProvider : public GameObject {
public:
    RandomIdentifierProvider();
    virtual ~RandomIdentifierProvider();
    
    int randomIdentifer();
};

#endif
