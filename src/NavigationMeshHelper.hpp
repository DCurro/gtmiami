#ifndef NavigationMeshHelper_HPP_
#define NavigationMeshHelper_HPP_

#include "GameObject.hpp"
#include "Common.hpp"

#include <SFML/Graphics.hpp>

class District;
class DistrictManager;
class PlayEntity;

class NavigationMeshHelper : public GameObject {
public:
    NavigationMeshHelper(sf::RenderWindow& window);
    virtual ~NavigationMeshHelper();
    
    void clearDistrictNavigationCellsOccupation(District* district);
    void setAppropriateCellsAsOccupied(b2World* world, DistrictManager* districtManager);
    
private:
    sf::RenderWindow& mWindow;
    
    void setNavCellWithPlayEntityForFixture(const b2Fixture* fixture,
                                            DistrictManager* districtManager,
                                            PlayEntity* playEntity);
    
//    void quadSearchFixtureAreaAndSetNavCellWithPlayEntity(const b2Fixture* fixture,
//                                                          const b2PolygonShape* navCellGroupShape,
//                                                          const b2Transform& navCellGroupTransform,
//                                                          DistrictManager* districtManager,
//                                                          PlayEntity* playEntity);
    
    float widthForPolygonShape(const b2PolygonShape* polygonShape);
//    
//    void rasterPolygon(const b2PolygonShape* polyshape,
//                       const b2Transform& transform,
//                       DistrictManager* districtManager,
//                       PlayEntity* playEntity);
//    
//    bool doPolygonsIntesect(const b2PolygonShape* shapeA,
//                            const b2Transform& transformA,
//                            const b2PolygonShape* shapeB,
//                            const b2Transform& transformB);
};

#endif
