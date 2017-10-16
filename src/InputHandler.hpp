#ifndef DRJINPUTHANDLER_HPP_
#define DRJINPUTHANDLER_HPP_

#include <SFML/Window.hpp>
#include <string>

#include "common.hpp"
#include "ControllerMap.hpp"

//#define INPUT_DEBUG //Uncomment to display debug information

class GameContext;

class InputHandler {
protected:
	int const BUTTON_DOWN = 100;
	int const BUTTON_UP = 0;

public:
	ACCESSOR(GameContext*, GameContext);

	GETTER(int, L1);
	GETTER(int, R1);
	GETTER(int, L2Position);
	GETTER(int, R2Position);
	GETTER(int, Up);
	GETTER(int, Down);
	GETTER(int, Left);
	GETTER(int, Right);
	GETTER(int, Start);
	GETTER(int, Select);
	GETTER(int, Button1);
	GETTER(int, Button2);
	GETTER(int, Button3);
	GETTER(int, Button4);
	sf::Vector2f getLeftJoystickPosition();
	sf::Vector2f getRightJoystickPosition();

	bool l1WasTapped();
	bool r1WasTapped();
	bool upWasTapped();
	bool downWasTapped();
	bool leftWasTapped();
	bool rightWasTapped();
	bool startWasTapped();
	bool selectWasTapped();
	bool button1WasTapped();
	bool button2WasTapped();
	bool button3WasTapped();
	bool button4WasTapped();
	bool l1WasTapped_consumeInput();
	bool r1WasTapped_consumeInput();
	bool upWasTapped_consumeInput();
	bool downWasTapped_consumeInput();
	bool leftWasTapped_consumeInput();
	bool rightWasTapped_consumeInput();
	bool startWasTapped_consumeInput();
	bool selectWasTapped_consumeInput();
	bool button1WasTapped_consumeInput();
	bool button2WasTapped_consumeInput();
	bool button3WasTapped_consumeInput();
	bool button4WasTapped_consumeInput();

private:
	sf::Vector2f mLeftJoystickPosition;
	sf::Vector2f mRightJoystickPosition;

	bool tappedL1;
	bool tappedR1;
	bool tappedUp;
	bool tappedDown;
	bool tappedLeft;
	bool tappedRight;
	bool tappedStart;
	bool tappedSelect;
	bool tappedButton1;
	bool tappedButton2;
	bool tappedButton3;
	bool tappedButton4;

	ControllerMap mControllerMap;

	void processStandardEvent(int eventValue, std::string standardEventString);
	sf::Vector2f debounceJoystick(sf::Vector2f joystickVector);
	std::string sfmlKeyCodeToString(sf::Keyboard::Key key);

public:
	InputHandler(GameContext* gameContext);
	virtual ~InputHandler();

	void update();
	void handleEvent(sf::Event event);
	void printControllerPretty();
};

#endif /* DRJINPUTHANDLER_HPP_ */
