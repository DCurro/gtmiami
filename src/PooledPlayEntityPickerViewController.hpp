#ifndef PooledPlayEntityPickerViewController_HPP_
#define PooledPlayEntityPickerViewController_HPP_

#include <SFGUI/SFGUI.hpp>

#include "common.hpp"
#include "ViewController.hpp"
#include "ResourceManager.hpp"

class PlayContext;

enum ButtonEntity {
    ButtonEntityZombie,
    ButtonEntityTurret,
    ButtonEntityCar,
    ButtonEntityHero,
    ButtonEntityTowTruck,
    ButtonEntityBunny,
    ButtonEntityAk47,
    ButtonEntityRocketLauncher,
    ButtonEntityRevolver,
    ButtonEntityGrenade,
};

class PooledPlayEntityPickerViewController : public ViewController {
public:
    PooledPlayEntityPickerViewController(PlayContext *playContext);
    virtual ~PooledPlayEntityPickerViewController();
    
    virtual void setupGUI();
    
protected:
    void buildPickerSection(sfg::Box::Ptr scrolledWindowBox);
    void onButtonClick(ButtonEntity buttonEntity);
    sfg::Button::Ptr createButton(ResourceManager::ImageResources imageResource, ButtonEntity buttonEntity);
};

#endif
