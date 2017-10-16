#include "catch.hpp"
#include "hippomocks.h"

#include "RandomIdentifierProvider.hpp"

TEST_CASE("RandomIdentifierProvider")
{
    HippoMocks::MockRepository mocks;
    
    RandomIdentifierProvider subject;
    
    SECTION("When generating a random integer")
    {
        int firstRandomInt = subject.randomIdentifer();
        int secondRandomInt = subject.randomIdentifer();
        
        INFO("it should be statistically different from the previous random integer") {
            CHECK(firstRandomInt != secondRandomInt);
        }
    }
}