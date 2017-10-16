#ifndef GUIModePlacePooledPlayObject_HPP_
#define GUIModePlacePooledPlayObject_HPP_

#include "GameObject.hpp"
#include "Common.hpp"
#include "GUIMode.hpp"
#include "PooledPlayEntityPickerViewController.hpp"

class GUIModePlacePooledPlayObject : public GUIMode {
public:
    GUIModePlacePooledPlayObject(PlayContext* playContext, ButtonEntity buttonEntity);
    virtual ~GUIModePlacePooledPlayObject();
    
    virtual void initialize() override;
    virtual void handleEvent(sf::Event event) override;
    virtual void destroy() override;
    virtual std::string description() override;
    
protected:
    ButtonEntity mButtonEntity;
};

#endif
