#include "catch.hpp"
#include "hippomocks.h"

#include "BadGuyMind.hpp"

TEST_CASE("BadGuyMind")
{
    HippoMocks::MockRepository mocks;
    
    BadGuyMind subject = new BadGuyMind();
    
    SECTION("When <#...#>")
    {
        subject.<#perform action#>();
        
        INFO("it should <#...#>") {
            CHECK(<#condition#>);
        }
    }
}