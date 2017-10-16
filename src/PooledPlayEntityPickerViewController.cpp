#include "PooledPlayEntityPickerViewController.hpp"

#include <SFML/Graphics.hpp>

#include "PlayContext.hpp"
#include "Game.hpp"
#import "GameContext.hpp"
#import "ResourceManager.hpp"
#import "Animation.hpp"
#include "GUIManager.hpp"
#include "GUIModePlacePooledPlayObject.hpp"

#define BUTTON_PADDING 1.0f
#define BUTTON_WIDTH 50.0f
#define BUTTON_HEIGHT BUTTON_WIDTH

PooledPlayEntityPickerViewController::PooledPlayEntityPickerViewController(PlayContext *playContext) : ViewController(playContext){
    
}

PooledPlayEntityPickerViewController::~PooledPlayEntityPickerViewController() {
    
}

void PooledPlayEntityPickerViewController::setupGUI()
{
    ViewController::setupGUI();
    
    mView->SetStyle(sfg::Window::Style::BACKGROUND);
    
    sfg::Box::Ptr viewBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);
    mView->Add(viewBox);
    
    sfg::Box::Ptr roadTilesBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);
    sfg::ScrolledWindow::Ptr roadTilesBoxScrolledWindow = sfg::ScrolledWindow::Create();
    sfg::Box::Ptr roadTilesScrolledWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, BUTTON_PADDING);
    roadTilesBoxScrolledWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS );
    roadTilesBoxScrolledWindow->AddWithViewport( roadTilesScrolledWindowBox );
    roadTilesBoxScrolledWindow->SetRequisition( sf::Vector2f( BUTTON_WIDTH+BUTTON_PADDING+100, SCREEN_HEIGHT-142 ) );
    roadTilesBox->Pack( roadTilesBoxScrolledWindow, false, true );
    
    viewBox->Pack(roadTilesBox);
    
    this->buildPickerSection(roadTilesScrolledWindowBox);
}

#pragma mark - Protected Methods

void PooledPlayEntityPickerViewController::buildPickerSection(sfg::Box::Ptr scrolledWindowBox)
{
    auto zombieButton = createButton(ResourceManager::ImageResources::zombieIdleCycle_png, ButtonEntityZombie);
    scrolledWindowBox->Pack(zombieButton);
    
    auto turretButton = createButton(ResourceManager::ImageResources::turretTop_png, ButtonEntityTurret);
    scrolledWindowBox->Pack(turretButton);
    
    auto carButton = createButton(ResourceManager::ImageResources::car_png, ButtonEntityCar);
    scrolledWindowBox->Pack(carButton);
    
    auto heroButton = createButton(ResourceManager::ImageResources::HeroIdleCycle_png, ButtonEntityHero);
    scrolledWindowBox->Pack(heroButton);
    
    auto towTruckButton = createButton(ResourceManager::ImageResources::towTruck_png, ButtonEntityTowTruck);
    scrolledWindowBox->Pack(towTruckButton);
    
    auto bunnyButton = createButton(ResourceManager::ImageResources::bunnyIdleCycle_png, ButtonEntityBunny);
    scrolledWindowBox->Pack(bunnyButton);
    
    auto ak47Button = createButton(ResourceManager::ImageResources::ak47_png, ButtonEntityAk47);
    scrolledWindowBox->Pack(ak47Button);
    
    auto rocketLauncherButton = createButton(ResourceManager::ImageResources::rocketLauncher_png, ButtonEntityRocketLauncher);
    scrolledWindowBox->Pack(rocketLauncherButton);
    
    auto revolverButton = createButton(ResourceManager::ImageResources::floorRevolver_png, ButtonEntityRevolver);
    scrolledWindowBox->Pack(revolverButton);
    
    auto grenadeButton = createButton(ResourceManager::ImageResources::grenadeStash_png, ButtonEntityGrenade);
    scrolledWindowBox->Pack(grenadeButton);
}

void PooledPlayEntityPickerViewController::onButtonClick(ButtonEntity buttonEntity){
    mClicked = true;
    
    GUIModePlacePooledPlayObject* guiMode = new GUIModePlacePooledPlayObject(mPlayContext, buttonEntity);
    this->getPlayContext()->getGUIManager()->setMode(guiMode);
}

sfg::Button::Ptr PooledPlayEntityPickerViewController::createButton(ResourceManager::ImageResources imageResource, ButtonEntity buttonEntity) {
    auto resourceManager = mPlayContext->getGameContext()->getResourceManager();
    
    sf::Texture texture = *Animation(false, 0, resourceManager->getImage(imageResource)).getCurrentFrame();
    
    sf::Sprite spriteTmp(texture);
    float scale = std::min(BUTTON_WIDTH/texture.getSize().x, BUTTON_HEIGHT/texture.getSize().y);
    spriteTmp.setScale(scale, scale); // TODO: scale 50.0f/texture-with-and-height
    
    sf::RenderTexture renderTexture;
    renderTexture.create(BUTTON_WIDTH,BUTTON_HEIGHT,false);
    renderTexture.clear();
    renderTexture.draw(spriteTmp);
    renderTexture.display();
    
    auto sfgImage = sfg::Image::Create(renderTexture.getTexture().copyToImage());
    
    auto button = sfg::Button::Create();
    button->SetImage(sfgImage);
    button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &PooledPlayEntityPickerViewController::onButtonClick, this, buttonEntity) );
    
    return button;
}
