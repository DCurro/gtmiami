#ifndef GUIMANAGER_HPP_
#define GUIMANAGER_HPP_

#include <vector>
#include <iostream>
#include <stack>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "common.hpp"

class GUIMode;
class PlayContext;
class ViewController;
class TilesPickerViewController;
class HintViewController;

class GUIManager : public sfg::Desktop {
protected:
	ACCESSOR(PlayContext*, PlayContext);
	ACCESSOR(bool, ShouldShowAndUpdate);

private:
	std::vector<ViewController*> viewControllers;
	HintViewController* mHintViewController;

	GUIMode* mGUIMode;

public:
	GUIManager(PlayContext* playContext);
	virtual ~GUIManager();

	void HandleEvent( const sf::Event& event );

	virtual void Update(float deltaTime);
	virtual void Display(sf::RenderWindow& target, sfg::SFGUI& sfgui);

	bool clickWasOnGUIWindow();

	void setMode(GUIMode* guiMode);

	void setHintText(std::string hintString);

private:
	void addViewController(ViewController* viewController);
	void clearMode();
};

#endif /* GUIMANAGER_HPP_ */
