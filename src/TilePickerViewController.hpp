#ifndef TILEPICKERVIEWCONTROLLER_HPP_
#define TILEPICKERVIEWCONTROLLER_HPP_

#include <SFGUI/SFGUI.hpp>

#include "common.hpp"
#include "ViewController.hpp"
#include "ResourceManager.hpp"

class PlayContext;

#define BUTTON_PADDING 1.0f
#define BUTTON_WIDTH 50.0f
#define BUTTON_HEIGHT BUTTON_WIDTH

#define SCROLL_OR_FRAME_SELECTED -1

class TilesPickerViewController : public ViewController {
protected:
    sfg::Notebook::Ptr mNotebook;
    
	int mCurrentTileID;
	std::string mCategoryString;
	bool mIsUsingBoxSelectMode;

public:
	TilesPickerViewController(PlayContext* playContext);
	virtual ~TilesPickerViewController();

	virtual void setupGUI();

private:
	void buildTilePickerSection(sfg::Box::Ptr scrolledWindowBox, ResourceManager::ImageResources imageResource, std::string categoryString);
    void onButtonClick(std::string categoryString, int buttonID);
    void radioButtonBoxSelection(bool boxSelectionWasSelected);

    bool isWallTabSetup = false;
    bool isInteriorTabSetup = false;
    void onNotebookLeftClick(sfg::Box::Ptr secondTabContentBox, sfg::Box::Ptr thirdTabContentBox);

};

#endif /* TILEPICKERVIEWCONTROLLER_HPP_ */
