#ifndef BUTTONGUI_HPP_
#define BUTTONGUI_HPP_

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "common.hpp"

#define CATEGORY_ROAD "road"
#define CATEGORY_WALL "wall"
#define CATEGORY_INTERIOR "interior"


class ButtonGUI : public sfg::Button {
protected:
	GETTER(std::string, CategoryString);
	GETTER(int, ButtonID);

public:
	typedef std::shared_ptr<ButtonGUI> Ptr; //!< Shared pointer.
	typedef std::shared_ptr<const ButtonGUI> PtrConst; //!< Shared pointer.

	ButtonGUI(std::string categoryString, int buttonID);
	virtual ~ButtonGUI();

	static Ptr Create(std::string categoryString, int buttonID);
};

#endif /* BUTTONGUI_HPP_ */
