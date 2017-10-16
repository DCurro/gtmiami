#include "ButtonGUI.hpp"

ButtonGUI::ButtonGUI(std::string categoryString, int buttonID) : sfg::Button(), mCategoryString(categoryString), mButtonID(buttonID){
}

ButtonGUI::~ButtonGUI(){
}

ButtonGUI::Ptr ButtonGUI::Create(std::string categoryString, int buttonID){
	ButtonGUI::Ptr button (new ButtonGUI(categoryString, buttonID));
	return button;
}
