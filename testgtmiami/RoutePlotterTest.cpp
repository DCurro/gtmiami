//
//#include "catch.hpp"
//#include "hippomocks.h"
//
//#include <vector>
//
//#include "RoutePlotter.hpp"
//#include "NavigationCell.hpp"
//
//TEST_CASE("RoutePlotter")
//{
//    HippoMocks::MockRepository mocks;
//    
//    RoutePlotter subject;
//    
//    SECTION("When plotting a route")
//    {
//        NavigationCell *center = mocks.Mock<NavigationCell>();
//        mocks.OnCall(center, NavigationCell::getNorthCell).Return(NULL);
//        mocks.OnCall(center, NavigationCell::getSouthCell).Return(NULL);
//        mocks.OnCall(center, NavigationCell::getEastCell).Return(NULL);
//        mocks.OnCall(center, NavigationCell::getWestCell).Return(NULL);
//        
//        SECTION("when plotting a route from a node to itself")
//        {   
//            std::vector<NavigationCell *> route = subject.plotRouteBetweenNavCells(center, center);
//            
//            INFO("It should have a route containing only itself") {
//                CHECK(route.size() == 1);
//            }
//        }
//        
//        SECTION("When plotting a route from a node to some other node")
//        {
//            NavigationCell *otherNode = mocks.Mock<NavigationCell>();
//            mocks.OnCall(center, NavigationCell::getNorthCell).Return(otherNode);
//            
//            mocks.OnCall(otherNode, NavigationCell::isOccupied).Return(false);
//            mocks.OnCall(otherNode, NavigationCell::getNorthCell).Return(NULL);
//            mocks.OnCall(otherNode, NavigationCell::getSouthCell).Return(center);
//            mocks.OnCall(otherNode, NavigationCell::getEastCell).Return(NULL);
//            mocks.OnCall(otherNode, NavigationCell::getWestCell).Return(NULL);
//            
//            mocks.OnCall(center, NavigationCell::getCenterPoint).Return(b2Vec2(0,0));
//            mocks.OnCall(otherNode, NavigationCell::getCenterPoint).Return(b2Vec2(0,0));
//            
//            SECTION("When the second node is unreachable from the first")
//            {
//                mocks.OnCall(otherNode, NavigationCell::isOccupied).Return(true);
//                
//                std::vector<NavigationCell *> route = subject.plotRouteBetweenNavCells(center, otherNode);
//                
//                INFO("It should not build a route") {
//                    CHECK(route.size() == 1);
//                }
//            }
//            
//            SECTION("When the second node is reachable from the first")
//            {
//                std::vector<NavigationCell *> route = subject.plotRouteBetweenNavCells(center, otherNode);
//                
//                INFO("It should have a plot a route from center node to otherNode") {
//                    CHECK(route.size() == 2);
//                    CHECK(route.at(0) == center);
//                    CHECK(route.at(1) == otherNode);
//                }
//            }
//            
////            TODO: need test utility to produce large number of cells connected
////            SECTION("When the second node is too far")
////            {
////                std::vector<NavigationCell *> route = subject.plotRouteBetweenNavCells(center, otherNode);
////                
////                INFO("It should not plot a route")
////                CHECK(route.empty() == true);
////            }
//            
//        }
//    }
//}