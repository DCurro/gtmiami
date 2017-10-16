#ifndef DebugManager_HPP_
#define DebugManager_HPP_

#include "GameObject.hpp"
#include "Common.hpp"

class DebugManager : public GameObject {
public:
    DebugManager();
    virtual ~DebugManager();
    
    void shouldDebugNavigationCellDebugging(bool shouldDebugNavigationCellDebugging);
};

#endif
