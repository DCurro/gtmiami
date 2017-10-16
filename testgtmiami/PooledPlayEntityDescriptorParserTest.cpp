#include "catch.hpp"
#include "hippomocks.h"

#include "PooledPlayEntityDescriptorParser.hpp"

TEST_CASE("PooledPlayEntityDescriptorParser")
{
    HippoMocks::MockRepository mocks;
    
    PooledPlayEntityDescriptorParser subject = new PooledPlayEntityDescriptorParser();
    
    SECTION("When <#...#>")
    {
        subject.<#perform action#>();
        
        INFO("it should <#...#>") {
            CHECK(<#condition#>);
        }
    }
}