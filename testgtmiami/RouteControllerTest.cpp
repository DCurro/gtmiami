#include "catch.hpp"
#include "hippomocks.h"

#include "RouteController.hpp"
#include "DistrictManager.hpp"

TEST_CASE("RouteController")
{
//    HippoMocks::MockRepository mocks;
//    
//    NavigationCell *navCell1 = mocks.Mock<NavigationCell>();
//    NavigationCell *navCell2 = mocks.Mock<NavigationCell>();
//    
//    std::vector<NavigationCell*> route;
//    route.push_back(navCell1);
//    route.push_back(navCell2);
//    
//    DistrictManager *districtManager = mocks.Mock<DistrictManager>();
//    mocks.OnCall(districtManager, DistrictManager::plotRouteFromStartToEnd).Return(route);
//    
//    PlayContext *playContext = mocks.Mock<PlayContext>();
//    
//    mocks.OnCall(playContext, PlayContext::getDistrictManager).Return(districtManager);
//    
//    Animal *animal = mocks.Mock<Animal>();
//    
//    RouteControllerDelegate* delegate = mocks.Mock<RouteControllerDelegate>();
//    
//    RouteController subject;
//    subject.setup(playContext, animal);
//    subject.delegate = delegate;
//    
//    SECTION("When setting a destination")
//    {
//        subject.setDestination(b2Vec2(10.0f,10.0f));
//        
//        INFO("it should plot a route") {
//            CHECK(false);
//        }
//    }
}