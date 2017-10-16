
#include "catch.hpp"
#include "hippomocks.h"

#include "ClassChecker.hpp"
#include "Bunny.hpp"
#include "PooledPlayEntity.hpp"
#include "NavigationCell.hpp"

TEST_CASE("ClassChecker")
{
    HippoMocks::MockRepository mocks;
    
    ClassChecker subject;
    
    SECTION("When checking a class kind")
    {
        bool isKindOfClass;
        
        Bunny* bunny = new Bunny();
        
        SECTION("and the class matches exactly") {
            isKindOfClass = subject.isKindOfClass<Bunny*>(bunny);
            
            INFO("it should return true") {
                CHECK(isKindOfClass == true);
            }
        }
        
        SECTION("and the class matching is a baser class") {
            isKindOfClass = subject.isKindOfClass<PooledPlayEntity*>(bunny);
            
            INFO("it should return true") {
                CHECK(isKindOfClass == true);
            }
        }
        
        SECTION("and the class doesn't match") {
            isKindOfClass = subject.isKindOfClass<NavigationCell*>(bunny);
            
            INFO("it should return false") {
                CHECK(isKindOfClass == false);
            }
        }
    }
}