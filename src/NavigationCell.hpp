#ifndef NAVIGATIONCELL_HPP_
#define NAVIGATIONCELL_HPP_

#include <SFML/Graphics.hpp>

#include "PlayContext.hpp"
#include "PlayEntity.hpp"
#include "common.hpp"

#include <set>

class NavigationCell : public PlayEntity {
protected:
    virtual Animation* newBulletStruckAnimation() override;
    virtual void struckByBulletConsequences(float bulletCalibre) override;
    void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;
    
public:    
    GETTER(b2Vec2, CenterPoint);
    GETTER(b2Vec2, BottomRightPoint);
    
    ACCESSOR(NavigationCell*, NorthCell);
    ACCESSOR(NavigationCell*, SouthCell);
    ACCESSOR(NavigationCell*, EastCell);
    ACCESSOR(NavigationCell*, WestCell);
    ACCESSOR(NavigationCell*, NorthEastCell);
    ACCESSOR(NavigationCell*, NorthWestCell);
    ACCESSOR(NavigationCell*, SouthEastCell);
    ACCESSOR(NavigationCell*, SouthWestCell);
    
    NavigationCell(PlayContext *playContext, b2Vec2 centerPoint, float widthAndHeight);
    virtual ~NavigationCell();
    
    virtual void draw(sf::RenderTarget& window);
    
    /** isOccupied
     *
     * Returns true if the cell is interacting with a physical object
     */
    virtual bool isOccupied();
    
    /** isOccupiedByAnythingOtherThanEntity
     *
     * Returns true if the cell is interacting with a physical object, 
     * other than the one specified
     */
    virtual bool isOccupiedByAnythingOtherThanEntity(PlayEntity* playEntity);
    
    /** is OccupiedByEntity
     *
     * Returns true if the cell is currently occupied by the specified play entity
     */
    virtual bool isOccupiedByEntity(PlayEntity *playEntity);

    virtual void occupyCell(PlayEntity* playEntity);
    virtual void unoccupyCell(PlayEntity* playEntity);
    virtual void clearOccupation();
   
    std::string description();
    
private:
    sf::Texture *mOccupiedTexture;
    
    std::set<PlayEntity*> mOccupierEntities;
};

#endif /* NAVIGATIONCELL_HPP_ */
