#include "NavigationMeshHelper.hpp"

#include <Box2D/Box2D.h>

#include "DistrictManager.hpp"
#include "District.hpp"
#include "NavigationCell.hpp"
#include "Automobile.hpp"
#include <vector>
#include <limits.h>

//inline double Dot(b2Vec2 a, b2Vec2 b){
//    return (a.x*b.x) + (a.y*b.y);
//}
//inline double PerpDot(b2Vec2 a, b2Vec2 b) {
//    return (a.y*b.x) - (a.x*b.y);
//}
//
//bool doLinesIntersect(b2Vec2 A1, b2Vec2 A2,
//                      b2Vec2 B1, b2Vec2 B2)
//{
//    b2Vec2 a(A2-A1);
//    b2Vec2 b(B2-B1);
//    
//    double f = PerpDot(a,b);
//    if(!f)      // lines are parallel
//        return false;
//    
//    b2Vec2 c(B2-A2);
//    double aa = PerpDot(a,c);
//    double bb = PerpDot(b,c);
//    
//    if(f < 0)
//    {
//        if(aa > 0)     return false;
//        if(bb > 0)     return false;
//        if(aa < f)     return false;
//        if(bb < f)     return false;
//    }
//    else
//    {
//        if(aa < 0)     return false;
//        if(bb < 0)     return false;
//        if(aa > f)     return false;
//        if(bb > f)     return false;
//    }
//    
//    return true;
//}
//
//bool NavigationMeshHelper::doPolygonsIntesect(const b2PolygonShape* shapeA,
//                                              const b2Transform& transformA,
//                                              const b2PolygonShape* shapeB,
//                                              const b2Transform& transformB)
//{
//    float angleA = transformA.q.GetAngle();
//    float angleB = transformB.q.GetAngle();
//    
//    for (int i=0; i<shapeA->GetVertexCount(); i++) {
//        int iNext = i+1;
//        if (i==shapeA->GetVertexCount()-1) {
//            iNext = 0;
//        }
//        
//        b2Vec2 a1 = shapeA->GetVertex(i);
//        b2Vec2 a2 = shapeA->GetVertex(iNext);
//        
//        b2Vec2 a1t(a1.x*cos(angleA) - a1.y*sin(angleA) + transformA.p.x,
//                   a1.x*sin(angleA) + a1.y*cos(angleA) + transformA.p.y);
//        b2Vec2 a2t(a2.x*cos(angleA) - a2.y*sin(angleA) + transformA.p.x,
//                   a2.x*sin(angleA) + a2.y*cos(angleA) + transformA.p.y);
//        
//        
//        for (int j=0; j<shapeB->GetVertexCount(); j++) {
//            int jNext = j+1;
//            if (j==shapeB->GetVertexCount()-1) {
//                jNext = 0;
//            }
//            
//            b2Vec2 b1 = shapeB->GetVertex(j);
//            b2Vec2 b2 = shapeB->GetVertex(jNext);
//            
//            b2Vec2 b1t(b1.x*cos(angleB) - b1.y*sin(angleB) + transformB.p.x,
//                       b1.x*sin(angleB) + b1.y*cos(angleB) + transformB.p.y);
//            b2Vec2 b2t(b2.x*cos(angleB) - b2.y*sin(angleB) + transformB.p.x,
//                       b2.x*sin(angleB) + b2.y*cos(angleB) + transformB.p.y);
//            
//            if (doLinesIntersect(a1t, a2t, b1t, b2t)) {
//                return true;
//            }
//        }
//    }
//    
//    return false;
//}
//
//bool pointInPolygon(float x, float y, int polyCorners, float polyX[], float polyY[]) {
//    
//    int   i, j=polyCorners-1 ;
//    bool  oddNodes=false      ;
//    
//    for (i=0; i<polyCorners; i++) {
//        if (((polyY[i]< y && polyY[j]>=y)
//             ||   (polyY[j]< y && polyY[i]>=y))
//            &&  (polyX[i]<=x || polyX[j]<=x)) {
//            oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
//        j=i; }
//    
//    return oddNodes;
//}
//
//bool isPointInPolygon(b2Vec2 point, b2PolygonShape* polyShape) {
//    float polyX[polyShape->GetVertexCount()];
//    float polyY[polyShape->GetVertexCount()];
//    
//    for (int i=0; i<polyShape->GetVertexCount(); i++) {
//        b2Vec2 vertex = polyShape->GetVertex(i);
//        polyX[i] = vertex.x;
//        polyY[i] = vertex.y;
//    }
//    
//    return pointInPolygon(point.x, point.y, polyShape->GetVertexCount(), polyX, polyY);
//}
//
//bool doBoxesIntersect(const b2AABB a, const b2AABB b)
//{
//    if (a.upperBound.x < b.lowerBound.x) return false; // a is left of b
//    if (a.lowerBound.x > b.upperBound.x) return false; // a is right of b
//    if (a.upperBound.y < b.lowerBound.y) return false; // a is above b
//    if (a.lowerBound.y > b.upperBound.y) return false; // a is below b
//    return true; // boxes overlap
//}

NavigationMeshHelper::NavigationMeshHelper(sf::RenderWindow& window) : mWindow(window) {

}

NavigationMeshHelper::~NavigationMeshHelper() {
    
}

void NavigationMeshHelper::clearDistrictNavigationCellsOccupation(District* district) {
    std::set<NavigationCell*>* navCells = district->mCellsThatHaveBeenOccupied;

    for (auto iter = navCells->begin(); iter != navCells->end(); iter++) {
        NavigationCell* navCell = *iter;
        navCell->clearOccupation();
    }
    
    district->mCellsThatHaveBeenOccupied->clear();
}

void NavigationMeshHelper::setAppropriateCellsAsOccupied(b2World* world, DistrictManager* districtManager) {
    for (b2Body* body = world->GetBodyList(); body!=NULL; body = body->GetNext()) {
        if (body->IsActive() ==  false) {
            continue;
        }

        for (b2Fixture* fixture = body->GetFixtureList(); fixture!=NULL; fixture = fixture->GetNext()) {
            if(fixture->IsSensor()) {
                continue; //sensors don't affect navigation.
            }

            PlayEntity* playEntity = NULL;

            if (fixture->GetUserData()) {
                playEntity = (PlayEntity*)fixture->GetUserData();
            } else if (body->GetUserData()) {
                playEntity = (PlayEntity*)body->GetUserData();
            }
            
            if (playEntity == NULL || ClassChecker().isKindOfClass<District*>(playEntity)) {
                continue;
            }
            
            //TODO: it might be better if we just combined the aabb's of all the fixutures on this body and used that
            //      instead of being specific about the type of class.
            if (ClassChecker().isKindOfClass<Automobile*>(playEntity)) {
                Automobile *automobile = (Automobile *)playEntity;
                if (fixture == automobile->getStaticBodyFixture()) {
                    continue;
                }
            }
            
            this->setNavCellWithPlayEntityForFixture(fixture, districtManager, playEntity);
        }
    }
//    std::cout << "iter" << std::endl;
}

#pragma mark - Private

float NavigationMeshHelper::widthForPolygonShape(const b2PolygonShape* polygonShape) {
    float leftMost = FLT_MAX;
    float rightMost = FLT_MIN;
    
    for (int i=0; i<polygonShape->GetVertexCount(); i++) {
        b2Vec2 vertex = polygonShape->GetVertex(i);
        leftMost = std::min(leftMost, vertex.x);
        rightMost = std::max(rightMost, vertex.x);
    }
    
    float returnWidth = rightMost-leftMost;
    
    return returnWidth;
}

//float roundToNextBaseTwo(float value) {
//    float rounded = 0;
//    
//    if (value > 1)
//    {
//        float f = (float)value;
//        unsigned int const t = 1U << ((*(unsigned int *)&f >> 23) - 0x7f);
//        rounded = t << (t < value);
//    }
//    else 
//    {
//        rounded = 1;
//    }
//    
//    return rounded;
//}

#define LEVERAGE 0.1

b2Vec2 rectifyNavCellTopLeft(float x, float y) {
    float xRectificationDifference = fmodf(x, 0.5f);
    if (xRectificationDifference < 0) {
        xRectificationDifference = 0.5f + xRectificationDifference;
    }
    
    float rectifiedX = x - xRectificationDifference;
    
    float yRectificationDifference = fmodf(y, 0.5f);
    if (yRectificationDifference < 0) {
        yRectificationDifference = 0.5f + yRectificationDifference;
    }
    
    float rectifiedY = y - yRectificationDifference;
    
    return b2Vec2(rectifiedX + 0.01, rectifiedY + 0.01);
}

void slideUpEdge(b2Vec2 lowerVertex, b2Vec2 upperVertex, DistrictManager *districtManager, PlayEntity *playEntity, sf::RenderWindow& window) {
    int numberOfVerticesToGenerate = (upperVertex.y-lowerVertex.y)/0.5f + 1;
    
    float xStride = (upperVertex.x-lowerVertex.x)/numberOfVerticesToGenerate;
    
    if (numberOfVerticesToGenerate == 0) {
        xStride = 0;
    }
    
    // Rectify last position
    float rectificationDifference = fmodf(lowerVertex.y, 0.5f);
    if (rectificationDifference < 0) {
        rectificationDifference = 0.5f + rectificationDifference;
    }
    float x = lowerVertex.x;
    float y = lowerVertex.y + rectificationDifference + 0.5f;
    
    sf::RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setOutlineThickness(2.0f);
    rectangle.setOutlineColor(sf::Color::Red);
    window.draw(rectangle);
    
    for (; y<upperVertex.y; y+=0.5) {
        b2Vec2 leverageUpEdge(rectifyNavCellTopLeft(x, y).x, rectifyNavCellTopLeft(x, y).y+LEVERAGE);
        b2Vec2 leverageDownEdge(rectifyNavCellTopLeft(x, y).x, rectifyNavCellTopLeft(x, y).y-LEVERAGE);
        
        districtManager->setNavigationCellOccupiedAtLocationWithEntity(leverageUpEdge, playEntity);
        districtManager->setNavigationCellOccupiedAtLocationWithEntity(leverageDownEdge, playEntity);
        
        x+=xStride;
    }
}

void slideRightEdge(b2Vec2 leftVertex, b2Vec2 rightVertex, DistrictManager *districtManager, PlayEntity *playEntity) {
    int numberOfVerticesToGenerate = (rightVertex.x-leftVertex.x)/0.5f + 1;
    
    float yStride = (rightVertex.y-leftVertex.y)/numberOfVerticesToGenerate;
    
    if (numberOfVerticesToGenerate == 0) {
        yStride = 0;
    }
    
    // Rectify last position
    float rectificationDifference = fmodf(leftVertex.x, 0.5f);
    if (rectificationDifference < 0) {
        rectificationDifference = 0.5f + rectificationDifference;
    }
    float x = leftVertex.x + rectificationDifference + 0.5f;
    float y = leftVertex.y;
    
    for (; x<rightVertex.x; x+=0.5) {
        b2Vec2 leverageUpEdge(rectifyNavCellTopLeft(x, y).x+LEVERAGE, rectifyNavCellTopLeft(x, y).y);
        b2Vec2 leverageDownEdge(rectifyNavCellTopLeft(x, y).x-LEVERAGE, rectifyNavCellTopLeft(x, y).y);
        
        districtManager->setNavigationCellOccupiedAtLocationWithEntity(leverageUpEdge, playEntity);
        districtManager->setNavigationCellOccupiedAtLocationWithEntity(leverageDownEdge, playEntity);
        
        y+=yStride;
    }
}

void NavigationMeshHelper::setNavCellWithPlayEntityForFixture(const b2Fixture *fixture, DistrictManager *districtManager, PlayEntity *playEntity) {
    
    //find the slope of the fixture starting from the bottom left
    const b2Shape* shape = fixture->GetShape();
    b2Transform transform = fixture->GetBody()->GetTransform();
    
    if (shape->GetType() == b2Shape::e_polygon)
    {
        b2PolygonShape* polyShape = (b2PolygonShape*)shape;
        
        for (int i=0; i<polyShape->GetVertexCount(); i++) {
            int iNext = i+1;
            if (i == polyShape->GetVertexCount()-1) {
                iNext = 0;
            }
            
            b2Vec2 vertexA(polyShape->GetVertex(i));
            b2Vec2 vertexB(polyShape->GetVertex(iNext));
            
            float angle = transform.q.GetAngle();
            b2Vec2 transformedVertexA = b2Vec2(vertexA.x*cos(angle) - vertexA.y*sin(angle) + transform.p.x,
                                               vertexA.x*sin(angle) + vertexA.y*cos(angle) + transform.p.y);
            b2Vec2 transformedVertexB = b2Vec2(vertexB.x*cos(angle) - vertexB.y*sin(angle) + transform.p.x,
                                               vertexB.x*sin(angle) + vertexB.y*cos(angle) + transform.p.y);
            
            b2Vec2 lowerVertex(transformedVertexA);
            b2Vec2 upperVertex(transformedVertexB);
            if (upperVertex.y < lowerVertex.y) {
                lowerVertex = transformedVertexB;
                upperVertex = transformedVertexA;
            }
            slideUpEdge(lowerVertex, upperVertex, districtManager, playEntity, mWindow);
            
            b2Vec2 leftVertex(transformedVertexA);
            b2Vec2 rightVertex(transformedVertexB);
            if (rightVertex.x < leftVertex.x) {
                leftVertex = transformedVertexB;
                rightVertex = transformedVertexA;
            }
//            slideRightEdge(leftVertex, rightVertex, districtManager, playEntity);
            
            //set vertexes
            districtManager->setNavigationCellOccupiedAtLocationWithEntity(lowerVertex, playEntity);
            districtManager->setNavigationCellOccupiedAtLocationWithEntity(upperVertex, playEntity);
        }
    }
    else if (shape->GetType() == b2Shape::e_circle)
    {
        
    }
}

//void NavigationMeshHelper::setNavCellWithPlayEntityForFixture(const b2Fixture* fixture,
//                                                              DistrictManager* districtManager,
//                                                              PlayEntity* playEntity)
//{
//    const b2Shape* fixtureShape = fixture->GetShape();
//    const b2Transform fixtureBodyTransform = fixture->GetBody()->GetTransform();
//    
//    if (fixtureShape->GetType() == b2Shape::e_polygon) {
//        const b2PolygonShape* polyShape = dynamic_cast<const b2PolygonShape*>(fixtureShape);
//        this->rasterPolygon(polyShape, fixtureBodyTransform, districtManager, playEntity);
//    }
//    
//}

//void NavigationMeshHelper::setNavCellWithPlayEntityForFixture(const b2Fixture* fixture,
//                                                              DistrictManager* districtManager,
//                                                              PlayEntity* playEntity)
//{
//    float navCellWidthHeight = District::METERES_PER_TILE/((float)NAVIGATION_CELLS_PER_TILE_WIDE);
//    
//    // 1) round down the top left corner to a multiple of the nav mesh
//    b2AABB navCellAlignedFixtureAABB(fixture->GetAABB(0));
//    float leftReductionValue = fmodf(navCellAlignedFixtureAABB.lowerBound.x, navCellWidthHeight);
//    if (leftReductionValue < 0) {
//        leftReductionValue = navCellWidthHeight + leftReductionValue;
//    }
//    float upReductionValue = fmodf(navCellAlignedFixtureAABB.lowerBound.y, navCellWidthHeight);
//    if (upReductionValue < 0) {
//        upReductionValue = navCellWidthHeight + upReductionValue;
//    }
//    navCellAlignedFixtureAABB.lowerBound = b2Vec2(navCellAlignedFixtureAABB.lowerBound.x-leftReductionValue,
//                                                  navCellAlignedFixtureAABB.lowerBound.y-upReductionValue);
//    
//    // 2) determine max between width and height of aabb
//    float maxOfWidthHeightOfAABB = fmaxf(navCellAlignedFixtureAABB.upperBound.x-navCellAlignedFixtureAABB.lowerBound.x,
//                                         navCellAlignedFixtureAABB.upperBound.y-navCellAlignedFixtureAABB.lowerBound.y);
//    float minOfWidthHeightOfAABB = fminf(navCellAlignedFixtureAABB.upperBound.x-navCellAlignedFixtureAABB.lowerBound.x,
//                                         navCellAlignedFixtureAABB.upperBound.y-navCellAlignedFixtureAABB.lowerBound.y);
//    
//    
//    // 3) round up the max side to a power of base 2
//    float baseTwoRoundedMinOfWidthHeight = roundToNextBaseTwo(minOfWidthHeightOfAABB);
//    
//    // 4) make the aabb a square based on the shortest side
//    b2AABB squareAABB(navCellAlignedFixtureAABB);
//    squareAABB.upperBound = b2Vec2(navCellAlignedFixtureAABB.lowerBound.x+baseTwoRoundedMinOfWidthHeight,
//                                   navCellAlignedFixtureAABB.lowerBound.y+baseTwoRoundedMinOfWidthHeight);
//    
//    //5) make the aabb's for the longer side
//    std::vector<b2AABB> aabbList;
//    
//    float rightStride = 0;
//    float downStride = 0;
//    
//    if (navCellAlignedFixtureAABB.upperBound.x-navCellAlignedFixtureAABB.lowerBound.x >
//        navCellAlignedFixtureAABB.upperBound.y-navCellAlignedFixtureAABB.lowerBound.y)
//    {
//        rightStride = baseTwoRoundedMinOfWidthHeight;
//    } else
//    {
//        downStride = baseTwoRoundedMinOfWidthHeight;
//    }
//    
//    int numberOfAABBs = ceilf(maxOfWidthHeightOfAABB/minOfWidthHeightOfAABB);
//    for (int i=0; i<numberOfAABBs; i++) {
//        b2AABB aabb;
//        aabb.lowerBound = b2Vec2(squareAABB.lowerBound.x + rightStride*i,
//                                 squareAABB.lowerBound.y + downStride*i);
//        aabb.upperBound = b2Vec2(squareAABB.upperBound.x + rightStride*i,
//                                 squareAABB.upperBound.y + downStride*i);
//        aabbList.push_back(aabb);
//    }
//    
//    //6) quad search on all of these aabbs
//    for (auto iter=aabbList.begin(); iter!=aabbList.end(); iter++) {
//        b2AABB aabb = *iter;
//        
//        b2PolygonShape navCellGroupShape;
//        navCellGroupShape.SetAsBox(baseTwoRoundedMinOfWidthHeight/2.0f, baseTwoRoundedMinOfWidthHeight/2.0f);
//        b2Transform navCellGroupTransform(b2Vec2(aabb.lowerBound.x+baseTwoRoundedMinOfWidthHeight/2.0f,
//                                                 aabb.lowerBound.y+baseTwoRoundedMinOfWidthHeight/2.0f),
//                                          b2Rot(0.0f));
//        
//        this->quadSearchFixtureAreaAndSetNavCellWithPlayEntity(fixture, &navCellGroupShape, navCellGroupTransform, districtManager, playEntity);
//    }
//}

//void NavigationMeshHelper::quadSearchFixtureAreaAndSetNavCellWithPlayEntity(const b2Fixture* fixture,
//                                                                            const b2PolygonShape* navCellGroupShape,
//                                                                            const b2Transform& navCellGroupTransform,
//                                                                            DistrictManager* districtManager,
//                                                                            PlayEntity* playEntity)
//{
//    float navCellGroupWidth = this->widthForPolygonShape(navCellGroupShape);
//    
//    //does overlap AABB?
//    b2AABB navCellGroupAABB = b2AABB();
//    navCellGroupAABB.lowerBound = navCellGroupTransform.p-b2Vec2(navCellGroupWidth/2.0f, navCellGroupWidth/2.0f);
//    navCellGroupAABB.upperBound = navCellGroupTransform.p+b2Vec2(navCellGroupWidth/2.0f, navCellGroupWidth/2.0f);
//    b2AABB fixtureAABB = fixture->GetAABB(0);
//    
//    if (doBoxesIntersect(fixtureAABB,navCellGroupAABB) == false) {
//        return;
//    }
//    
//    //yes; Is smallest cell size?
//    float smallestSize = District::METERES_PER_TILE/((float)NAVIGATION_CELLS_PER_TILE_WIDE);
//    
//    if (navCellGroupWidth <= smallestSize) {
//        const b2Shape* fixtureShape = fixture->GetShape();
//        const b2Transform fixtureBodyTransform = fixture->GetBody()->GetTransform();
//        
//        if (b2TestOverlap(fixtureShape, 0, navCellGroupShape, 0, fixtureBodyTransform, navCellGroupTransform) == true) {
//            b2Vec2 location = navCellGroupTransform.p;
//            districtManager->setNavigationCellOccupiedAtLocationWithEntity(location, playEntity); //yes
//        }
//        
//        return;
//    }
//    
//    //no; recurse on 4 smallers subsets
//    float halfLeft = -navCellGroupWidth/4.0f;
//    float halfRight = navCellGroupWidth/4.0f;
//    float halfUp = -navCellGroupWidth/4.0f;
//    float halfDown = navCellGroupWidth/4.0f;
//    
//    b2Vec2 topLeftVec(halfLeft, halfUp);
//    b2Vec2 topRightVec(halfRight, halfUp);
//    b2Vec2 bottomLeftVec(halfLeft, halfDown);
//    b2Vec2 bottomRightVec(halfRight, halfDown);
//    
//    b2Transform navCellGroupTransformTopLeft(navCellGroupTransform.p+topLeftVec, navCellGroupTransform.q);
//    b2Transform navCellGroupTransformTopRight(navCellGroupTransform.p+topRightVec, navCellGroupTransform.q);
//    b2Transform navCellGroupTransformBottomLeft(navCellGroupTransform.p+bottomLeftVec, navCellGroupTransform.q);
//    b2Transform navCellGroupTransformBottomRight(navCellGroupTransform.p+bottomRightVec, navCellGroupTransform.q);
//    
//    b2PolygonShape halfNavCellGroupShape;
//    halfNavCellGroupShape.SetAsBox(navCellGroupWidth/4.0f, navCellGroupWidth/4.0f);
//    
//    this->quadSearchFixtureAreaAndSetNavCellWithPlayEntity(fixture,
//                                                           &halfNavCellGroupShape,
//                                                           navCellGroupTransformTopLeft,
//                                                           districtManager,
//                                                           playEntity);
//    this->quadSearchFixtureAreaAndSetNavCellWithPlayEntity(fixture,
//                                                           &halfNavCellGroupShape,
//                                                           navCellGroupTransformTopRight,
//                                                           districtManager,
//                                                           playEntity);
//    this->quadSearchFixtureAreaAndSetNavCellWithPlayEntity(fixture,
//                                                           &halfNavCellGroupShape,
//                                                           navCellGroupTransformBottomLeft,
//                                                           districtManager,
//                                                           playEntity);
//    this->quadSearchFixtureAreaAndSetNavCellWithPlayEntity(fixture,
//                                                           &halfNavCellGroupShape,
//                                                           navCellGroupTransformBottomRight,
//                                                           districtManager,
//                                                           playEntity);
//}

//void NavigationMeshHelper::rasterPolygon(const b2PolygonShape* polyshape,
//                                         const b2Transform& transform,
//                                         DistrictManager* districtManager,
//                                         PlayEntity* playEntity)
//{
//    b2AABB aabb;
//    polyshape->ComputeAABB(&aabb, transform, 0);
//    
//    
//    b2AABB navCellAlignedFixtureAABB(aabb);
//    float leftReductionValue = fmodf(navCellAlignedFixtureAABB.lowerBound.x, 0.5f);
//    if (leftReductionValue < 0) {
//        leftReductionValue = 0.5f + leftReductionValue;
//    }
//    float upReductionValue = fmodf(navCellAlignedFixtureAABB.lowerBound.y, 0.5f);
//    if (upReductionValue < 0) {
//        upReductionValue = 0.5f + upReductionValue;
//    }
//    navCellAlignedFixtureAABB.lowerBound = b2Vec2(navCellAlignedFixtureAABB.lowerBound.x-leftReductionValue,
//                                                  navCellAlignedFixtureAABB.lowerBound.y-upReductionValue);
//    
//    float leftBound = navCellAlignedFixtureAABB.lowerBound.x;
//    float rightBound = navCellAlignedFixtureAABB.upperBound.x;
//    float topBound = navCellAlignedFixtureAABB.lowerBound.y;
//    float bottomBound = navCellAlignedFixtureAABB.upperBound.y;
//
//    for (float x = leftBound; x<=rightBound; x+=0.25f) {
//        for (float y = topBound; y<=bottomBound; y+=0.25f) {
//            b2PolygonShape collisionCell;
//            collisionCell.SetAsBox(0.25, 0.25);
//            
//            if(doPolygonsIntesect(polyshape, transform, &collisionCell, b2Transform(b2Vec2(x,y), b2Rot(0))))
//            {
//                districtManager->setNavigationCellOccupiedAtLocationWithEntity(b2Vec2(x,y), playEntity);
//            };
//        }
//    }
//}

//void NavigationMeshHelper::rasterPolygon(const b2PolygonShape* polyshape,
//                                         const b2Transform& transform,
//                                         DistrictManager* districtManager,
//                                         PlayEntity* playEntity)
//{
//    int polyVertexCount = polyshape->m_count;
//    
//    
//    float  polyX[polyVertexCount];
//    float  polyY[polyVertexCount];
//    
//    for (int i=0; i<polyshape->m_count; i++) {
//        float angle = transform.q.GetAngle();
//        float x = polyshape->m_vertices[i].x;
//        float y = polyshape->m_vertices[i].y;
//        
//        polyX[i] = x*cos(angle) - y*sin(angle) + transform.p.x;
//        polyY[i] = x*sin(angle) + y*cos(angle) + transform.p.y;
//    }
//    
//    float leastX = FLT_MAX;
//    float mostX = -FLT_MAX;
//    float leastY = FLT_MAX;
//    float mostY = -FLT_MAX;
//    
//    for (int i=0; i<polyshape->m_count; i++) {
//        float x = polyX[i];
//        float y = polyY[i];
//
//        if (x > mostX) {
//            mostX = x;
//        }
//        
//        if (x < leastX) {
//            leastX = x;
//        }
//        
//        if (y > mostY) {
//            mostY = y;
//        }
//        
//        if (y < leastY) {
//            leastY = y;
//        }
//    }
//    
//    float nodeX[1000];
//    
//    b2AABB aabb;
//    polyshape->ComputeAABB(&aabb, transform, 0);
//    
//    float leftBound = aabb.lowerBound.x - 1.0f;// leastX;
//    float rightBound = aabb.upperBound.x + 1.0f;// mostX;
//    float topBound = aabb.lowerBound.y - 1.0f;// leastY;
//    float bottomBound = aabb.upperBound.y + 1.0f;// mostY;
//    
//    //  Loop through the rows of the image.
//    for (float pixelY=topBound; pixelY<=bottomBound; pixelY+=0.125f) { //nav cell width/height
//        
//        //  Build a list of nodes.
//        int nodeCount = 0;
//        int j = polyVertexCount-1;
//        for (int i=0; i<polyVertexCount; i++)
//        {
//            float y = polyY[i];
//            
//            if ((polyY[i]<=pixelY && polyY[j]>=pixelY) ||
//                (polyY[j]<=pixelY && polyY[i]>=pixelY))
//            {
//                float x = (polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i]));
//                nodeX[nodeCount++]=(polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i]));
//            }
//            
//            j=i;
//        }
//        
//        //  Sort the nodes, via a simple “Bubble” sort.
//        int i = 0;
//        while (i<nodeCount-1)
//        {
//            if (nodeX[i]>nodeX[i+1])
//            {
//                float swap;
//                
//                swap=nodeX[i];
//                nodeX[i]=nodeX[i+1];
//                nodeX[i+1]=swap;
//                if (i) i--;
//            }
//            else
//            {
//                i++;
//            }
//        }
//        
//        //  Fill the pixels between node pairs.
//        for (int i=0; i<nodeCount; i+=2)
//        {
//            if   (nodeX[i]>=rightBound) {
//                break;
//            }
//            
//            if   (nodeX[i+1]>=leftBound )
//            {
//                if (nodeX[i]<=leftBound ) {
//                    nodeX[i]=leftBound ;
//                }
//                
//                if (nodeX[i+1]>=rightBound) {
//                    nodeX[i+1]=rightBound;
//                }
//                
//                for (float pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX+=0.5) {
//                    districtManager->setNavigationCellOccupiedAtLocationWithEntity(b2Vec2(pixelX,pixelY), playEntity);
//                }
//            }
//        }
//    }
//}
