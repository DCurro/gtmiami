#include "GUIModePlacePooledPlayObject.hpp"
#include "GUIManager.hpp"
#include "PlayContext.hpp"
#include "Camera.hpp"
#include "Factory.hpp"
#include "EntityManager.hpp"
#include "DistrictManager.hpp"
#include "BunnyDescriptor.hpp"
#include "RandomIdentifierProvider.hpp"
#include "ZombieDescriptor.hpp"
#include "TurretDescriptor.hpp"

GUIModePlacePooledPlayObject::GUIModePlacePooledPlayObject(PlayContext* playContext, ButtonEntity buttonEntity)
: GUIMode(playContext), mButtonEntity(buttonEntity) {

}

GUIModePlacePooledPlayObject::~GUIModePlacePooledPlayObject() {
    
}

void GUIModePlacePooledPlayObject::initialize() {
    mPlayContext->getGUIManager()->setHintText("Click anywhere in the world to place an entity.");
}

void GUIModePlacePooledPlayObject::handleEvent(sf::Event event) {
    if(mPlayContext->getGUIManager()->clickWasOnGUIWindow()){
        return;
    }
    
    switch( event.type ){
        case sf::Event::MouseButtonReleased : {
            if(event.mouseButton.button == sf::Mouse::Button::Left){
                sf::Vector2f mouseCoords(event.mouseButton.x,event.mouseButton.y);
                sf::Vector2f clickLocationWorld = mPlayContext->getCamera()->screenToWorld_relative(mouseCoords);
                
                Camera* camera = mPlayContext->getCamera();
                b2Vec2 worldPosition(clickLocationWorld.x, clickLocationWorld.y);
                
                switch (mButtonEntity) {
                    case ButtonEntityZombie: {
                        auto descriptor = new ZombieDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityTurret: {
                        auto descriptor = new TurretDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityCar: {
                        auto descriptor = new CarDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityHero: {
                        auto hero = this->getPlayContext()->getFactory()->createHero(this->mPlayContext,
                                                                                     camera->getCurrentFloor(),
                                                                                     worldPosition,
                                                                                     0.0f);
                        this->getPlayContext()->getEntityManager()->addEntity(hero);
                        break;
                    }
                        
                    case ButtonEntityTowTruck: {
                        auto descriptor = new TowTruckDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityBunny: {
                        auto bunnyDescriptor = new BunnyDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(bunnyDescriptor);
                        break;
                    }
                        
                    case ButtonEntityAk47: {
                        auto descriptor = new Ak47Descriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityRocketLauncher: {
                        auto descriptor = new RocketLauncherDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityRevolver: {
                        auto descriptor = new RevolverDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                        
                    case ButtonEntityGrenade: {
                        auto descriptor = new GrenadeDescriptor(RandomIdentifierProvider().randomIdentifer(), camera->getCurrentFloor(), worldPosition.x, worldPosition.y, 0.0f);
                        
                        this->getPlayContext()->getDistrictManager()->setPooledPlayEntityForDescriptor(descriptor);
                        break;
                    }
                
                    default: {
                        assert(false && "unhandled type");
                        break;
                    }
                }
            }
        } break;
            
        default:
            break;
    }
}

void GUIModePlacePooledPlayObject::destroy() {
    mPlayContext->getGUIManager()->setHintText("Click anywhere in the world to place an entity.");
}

std::string GUIModePlacePooledPlayObject::description() {
    return std::string("");
}