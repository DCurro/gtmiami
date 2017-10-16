#include "TilePickerViewController.hpp"

#include <functional>
#include <iostream>
#include <string>

#include "ButtonGUI.hpp"
#include "Game.hpp"
#include "GameContext.hpp"
#include "GUIManager.hpp"
#include "GUIModeBoxPlaceTiles.hpp"
#include "GUIModeTracePlaceTiles.hpp"
#include "PlayContext.hpp"

sf::Image halfSizeImage(sf::Image image){
	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite spriteTmp(texture);
	spriteTmp.setScale(0.5f,0.5f);
	sf::RenderTexture renderTexture;
	renderTexture.create(BUTTON_WIDTH,BUTTON_HEIGHT,false);
	renderTexture.clear();
	renderTexture.draw(spriteTmp);
	renderTexture.display();

	return renderTexture.getTexture().copyToImage();
}

TilesPickerViewController::TilesPickerViewController(PlayContext* playContext) : ViewController(playContext) {
	mIsUsingBoxSelectMode = true;
	mCurrentTileID = 0;
}

TilesPickerViewController::~TilesPickerViewController() {
}

void TilesPickerViewController::setupGUI() {
	ViewController::setupGUI();

	mView->SetStyle(sfg::Window::Style::BACKGROUND);

	sfg::Box::Ptr viewBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);

	mView->Add(viewBox);

    mNotebook = sfg::Notebook::Create();

	sfg::Box::Ptr roadTilesBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);
	sfg::ScrolledWindow::Ptr roadTilesBoxScrolledWindow = sfg::ScrolledWindow::Create();
	sfg::Box::Ptr roadTilesScrolledWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, BUTTON_PADDING);
	roadTilesBoxScrolledWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS );
	roadTilesBoxScrolledWindow->AddWithViewport( roadTilesScrolledWindowBox );
	roadTilesBoxScrolledWindow->SetRequisition( sf::Vector2f( BUTTON_WIDTH+BUTTON_PADDING+100, SCREEN_HEIGHT-142 ) );
	roadTilesBox->Pack( roadTilesBoxScrolledWindow, false, true );

	sfg::Box::Ptr wallsTilesBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);
	sfg::ScrolledWindow::Ptr wallsTilesBoxScrolledWindow = sfg::ScrolledWindow::Create();
	sfg::Box::Ptr wallsTilesScrolledWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, BUTTON_PADDING);
	wallsTilesBoxScrolledWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS );
	wallsTilesBoxScrolledWindow->AddWithViewport( wallsTilesScrolledWindowBox );
	wallsTilesBoxScrolledWindow->SetRequisition( sf::Vector2f( BUTTON_WIDTH+BUTTON_PADDING+100, SCREEN_HEIGHT-142 ) );
	wallsTilesBox->Pack( wallsTilesBoxScrolledWindow, false, true );

	sfg::Box::Ptr interiorTilesBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.0f);
	sfg::ScrolledWindow::Ptr interiorTilesBoxScrolledWindow = sfg::ScrolledWindow::Create();
	sfg::Box::Ptr interiorTilesScrolledWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, BUTTON_PADDING);
	interiorTilesBoxScrolledWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS );
	interiorTilesBoxScrolledWindow->AddWithViewport( interiorTilesScrolledWindowBox );
	interiorTilesBoxScrolledWindow->SetRequisition( sf::Vector2f( BUTTON_WIDTH+BUTTON_PADDING+100, SCREEN_HEIGHT-142 ) );
	interiorTilesBox->Pack( interiorTilesBoxScrolledWindow, false, true );

	mNotebook->SetScrollable(true);
    
    mNotebook->AppendPage(roadTilesBox, sfg::Label::Create("Road"));
    mNotebook->AppendPage(wallsTilesBox, sfg::Label::Create("Walls"));
	mNotebook->AppendPage(interiorTilesBox, sfg::Label::Create("Interior"));
    
    this->buildTilePickerSection(roadTilesScrolledWindowBox, ResourceManager::ImageResources::tilemap_png, CATEGORY_ROAD);
    
    mNotebook->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &TilesPickerViewController::onNotebookLeftClick, this, wallsTilesScrolledWindowBox, interiorTilesScrolledWindowBox) );

	sfg::Box::Ptr radioBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL);
	sfg::RadioButton::Ptr boxSelectRadioBtn = sfg::RadioButton::Create("Box");
	boxSelectRadioBtn->SetActive(true);
	boxSelectRadioBtn->GetSignal( sfg::Widget::OnMouseLeftPress ).Connect( std::bind( &TilesPickerViewController::radioButtonBoxSelection, this, true ) );
	sfg::RadioButton::Ptr traceSelectRadioBtn = sfg::RadioButton::Create("Trace", boxSelectRadioBtn->GetGroup());
	traceSelectRadioBtn->GetSignal( sfg::Widget::OnMouseLeftPress ).Connect( std::bind( &TilesPickerViewController::radioButtonBoxSelection, this, false ) );
	radioBox->Pack( boxSelectRadioBtn );
	radioBox->Pack( traceSelectRadioBtn );

	viewBox->Pack(mNotebook);
	viewBox->Pack(radioBox);
}

#pragma mark - Private Helpers

void TilesPickerViewController::buildTilePickerSection(sfg::Box::Ptr scrolledWindowBox, ResourceManager::ImageResources imageResource, std::string categoryString) {
    sf::Image* tileMap = mPlayContext->getGameContext()->getResourceManager()->getImage(imageResource);

	for(int y=0; y<10; y++){
		for(int x=0; x<5; x++){
			sfg::Box::Ptr imageBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, BUTTON_PADDING);
			for(int z=0; z<=1; z++){
				sf::Image tileImage;
				tileImage.create(100,100);
				tileImage.copy(*tileMap,0,0,sf::IntRect((x*2+z)*100,y*100,100,100),false);
				sfg::Image::Ptr sfgImage = sfg::Image::Create( halfSizeImage(tileImage) );
				int id = y*10 + x*2 + z;
				ButtonGUI::Ptr button = ButtonGUI::Create( categoryString, id );
				button->GetSignal( sfg::Widget::OnMouseLeftPress ).Connect( std::bind( &TilesPickerViewController::onButtonClick, this, button->getCategoryString(), button->getButtonID() ) );
				button->SetImage( sfgImage );
                
				imageBox->Pack(button);
			}
			scrolledWindowBox->Pack(imageBox);
		}
	}
}

#pragma mark - UI Actions

void TilesPickerViewController::onButtonClick(std::string categoryString, int buttonID){
	std::cout << buttonID << std::endl;
	mCurrentTileID = buttonID;
	mCategoryString = categoryString;
	mClicked = true;
	if(buttonID != SCROLL_OR_FRAME_SELECTED){
		if(mIsUsingBoxSelectMode){
			mPlayContext->getGUIManager()->setMode(new GUIModeBoxPlaceTiles(mPlayContext, mCategoryString, mCurrentTileID));
		} else {
			mPlayContext->getGUIManager()->setMode(new GUIModeTracePlaceTiles(mPlayContext, mCategoryString, mCurrentTileID));
		}
	}
}

void TilesPickerViewController::radioButtonBoxSelection(bool boxSelectionWasSelected){
	mClicked = true;
	mIsUsingBoxSelectMode = boxSelectionWasSelected;
	this->onButtonClick(mCategoryString, mCurrentTileID);
}


void TilesPickerViewController::onNotebookLeftClick(sfg::Box::Ptr secondTabContentBox, sfg::Box::Ptr thirdTabContentBox) {

    // lazily load the tab groups
    
    switch (mNotebook->GetCurrentPage()) {
        case 0:
            //this is already loaded
            break;
            
        case 1:
            if (isWallTabSetup == false) {
                isWallTabSetup = true;
                this->buildTilePickerSection(secondTabContentBox, ResourceManager::ImageResources::wallTilemap_png, CATEGORY_WALL);
            }
            break;
            
        case 2:
            if (isInteriorTabSetup == false) {
                isInteriorTabSetup = true;
                this->buildTilePickerSection(thirdTabContentBox, ResourceManager::ImageResources::interiorTilemap_png, CATEGORY_INTERIOR);
            }
            break;
            
        default:
            assert(false && "you failed to set up the new tab section");
            break;
    }
}
