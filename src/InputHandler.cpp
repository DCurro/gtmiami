#include "InputHandler.hpp"

#include <iostream>
#include <sstream>

#include "GameContext.hpp"

InputHandler::InputHandler(GameContext* gameContext) : mGameContext(gameContext) {
	mL1 = 0;
	mL2Position = 0;
	mR1 = 0;
	mR2Position = 0;
	mUp = 0;
	mDown = 0;
	mLeft = 0;
	mRight = 0;
	mStart = 0;
	mSelect = 0;
	mButton1 = 0;
	mButton2 = 0;
	mButton3 = 0;
	mButton4 = 0;
	mLeftJoystickPosition = sf::Vector2f(0,0);
	mRightJoystickPosition = sf::Vector2f(0,0);

	tappedL1 = false;
	tappedR1 = false;
	tappedUp = false;
	tappedDown = false;
	tappedLeft = false;
	tappedRight = false;
	tappedStart = false;
	tappedSelect = false;
	tappedButton1 = false;
	tappedButton2 = false;
	tappedButton3 = false;
	tappedButton4 = false;
}

InputHandler::~InputHandler() {
}

sf::Vector2f InputHandler::getLeftJoystickPosition(){
	return debounceJoystick(mLeftJoystickPosition);
}

sf::Vector2f InputHandler::getRightJoystickPosition(){
	return debounceJoystick(mRightJoystickPosition);
}

/* debounceJoystick
 *
 * Returns a zero-vector if the joystick vector falls below the debouncing length,
 * otherwise returns the joystickVector.
 *
 */
sf::Vector2f InputHandler::debounceJoystick(const sf::Vector2f joystickVector){
	sf::Vector2f debouncedVector;
	if( std::sqrt(joystickVector.x*joystickVector.x + joystickVector.y*joystickVector.y) < AXIS_DEADZONE_RADIUS ){
		debouncedVector =  sf::Vector2f(0,0);
	} else {
		debouncedVector = joystickVector;
	}
	return debouncedVector;
}

void InputHandler::update() {
	tappedL1 = false;
	tappedR1 = false;
	tappedUp = false;
	tappedDown = false;
	tappedLeft = false;
	tappedRight = false;
	tappedStart = false;
	tappedSelect = false;
	tappedButton1 = false;
	tappedButton2 = false;
	tappedButton3 = false;
	tappedButton4 = false;
}

/* handleEvent
 *
 * Processes an sf::Event as a user custom event, and sets the appropriate standard controls
 */
void InputHandler::handleEvent(sf::Event event) {
	switch(event.type){
		case sf::Event::JoystickConnected: {
			mControllerMap.saveCustomLayout();
			mControllerMap.findController();
		} break;
		case sf::Event::JoystickDisconnected: {
			mControllerMap.saveCustomLayout();
			mControllerMap.findController();
		} break;

		case sf::Event::JoystickMoved: {
			if (event.joystickMove.axis == sf::Joystick::X) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("X_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("X_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::Y) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("Y_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("Y_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::Z) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("Z_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("Z_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::R) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("R_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("R_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::U) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("U_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("U_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::V) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("V_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("V_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::PovX) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("POVX_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("POVX_AXIS+") );
				}
			}
			if (event.joystickMove.axis == sf::Joystick::PovY) {
				if(event.joystickMove.position < 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("POVY_AXIS-") );
				} else if(event.joystickMove.position >= 0) {
					this->processStandardEvent(event.joystickMove.position,this->mControllerMap.inputControlToStandardControl("POVY_AXIS+") );
				}
			}
		} break;

		case sf::Event::JoystickButtonPressed : {
			std::stringstream ss;
			ss << "BUTTON" << event.joystickButton.button;
			this->processStandardEvent(BUTTON_DOWN ,this->mControllerMap.inputControlToStandardControl(ss.str()) );
		} break;

		case sf::Event::JoystickButtonReleased : {
			std::stringstream ss;
			ss << "BUTTON" << event.joystickButton.button;
			this->processStandardEvent(BUTTON_UP ,this->mControllerMap.inputControlToStandardControl(ss.str()) );
		} break;

		case sf::Event::KeyPressed : {
			std::string input = sfmlKeyCodeToString(event.key.code);
			this->processStandardEvent(BUTTON_DOWN ,this->mControllerMap.inputControlToStandardControl(input));

		} break;
		case sf::Event::KeyReleased : {
			std::string input = sfmlKeyCodeToString(event.key.code);
			this->processStandardEvent(BUTTON_UP ,this->mControllerMap.inputControlToStandardControl(input));
		} break;

		default:
			break;
	}
}

/* processStandardEvent
 *
 * sets data according to the eventValue (internally normalized to positive 100) and the standard event string.
 * This is a helper function of handleEvent
 */
void InputHandler::processStandardEvent(const int eventValue, std::string standardEventString){
	const int normalizedEventValue = std::abs(eventValue);

	if(standardEventString.compare("BUTTON1") == 0){
		if(mButton1>0 && normalizedEventValue==0) {
			tappedButton1 = true;
		}
		mButton1 = normalizedEventValue;
	} else if(standardEventString.compare("BUTTON2") == 0){
		if(mButton2>0 && normalizedEventValue==0) {
			tappedButton2 = true;
		}
		mButton2 = normalizedEventValue;
	} else if(standardEventString.compare("BUTTON3") == 0){
		if(mButton3>0 && normalizedEventValue==0) {
			tappedButton3 = true;
		}
		mButton3 = normalizedEventValue;
	} else if(standardEventString.compare("BUTTON4") == 0){
		if(mButton4>0 && normalizedEventValue==0) {
			tappedButton4 = true;
		}
		mButton4 = normalizedEventValue;
	} else if(standardEventString.compare("UP") == 0){
		if(mUp>0 && normalizedEventValue==0) {
			tappedUp = true;
		}
		mUp = normalizedEventValue;
	} else if(standardEventString.compare("DOWN") == 0){
		if(mDown>0 && normalizedEventValue==0) {
			tappedDown = true;
		}
		mDown = normalizedEventValue;
	} else if(standardEventString.compare("LEFT") == 0){
		if(mLeft>0 && normalizedEventValue==0) {
			tappedLeft = true;
		}
		mLeft = normalizedEventValue;
	} else if(standardEventString.compare("RIGHT") == 0){
		if(mRight>0 && normalizedEventValue==0) {
			tappedRight = true;
		}
		mRight = normalizedEventValue;
	} else if(standardEventString.compare("START") == 0){
		if(mStart>0 && normalizedEventValue==0) {
			tappedStart = true;
		}
		mStart = normalizedEventValue;
	} else if(standardEventString.compare("SELECT") == 0){
		if(mSelect>0 && normalizedEventValue==0) {
			tappedSelect = true;
		}
		mSelect = normalizedEventValue;
	} else if(standardEventString.compare("JOY1_UP_AXIS") == 0){
		mLeftJoystickPosition.y = -normalizedEventValue;
	} else if(standardEventString.compare("JOY1_DOWN_AXIS") == 0){
		mLeftJoystickPosition.y = normalizedEventValue;
	} else if(standardEventString.compare("JOY1_LEFT_AXIS") == 0){
		mLeftJoystickPosition.x = -normalizedEventValue;
	} else if(standardEventString.compare("JOY1_RIGHT_AXIS") == 0){
		mLeftJoystickPosition.x = normalizedEventValue;
	} else if(standardEventString.compare("JOY2_UP_AXIS") == 0){
		mRightJoystickPosition.y = -normalizedEventValue;
	} else if(standardEventString.compare("JOY2_DOWN_AXIS") == 0){
		mRightJoystickPosition.y = normalizedEventValue;
	} else if(standardEventString.compare("JOY2_LEFT_AXIS") == 0){
		mRightJoystickPosition.x = -normalizedEventValue;
	} else if(standardEventString.compare("JOY2_RIGHT_AXIS") == 0){
		mRightJoystickPosition.x = normalizedEventValue;
	} else if(standardEventString.compare("L1_BUTTON") == 0){
		if(mL1>0 && normalizedEventValue==0) {
			tappedL1 = true;
		}
		mL1 = normalizedEventValue;
	} else if(standardEventString.compare("L2_AXIS") == 0){
		mL2Position = normalizedEventValue;
	} else if(standardEventString.compare("R1_BUTTON") == 0){
		if(mR1>0 && normalizedEventValue==0) {
			tappedR1 = true;
		}
		mR1 = normalizedEventValue;
	} else if(standardEventString.compare("R2_AXIS") == 0){
		mR2Position = normalizedEventValue;
	} else {
//		std::cerr << "UNKNOWN CONTROLLER EVENT" << standardEventString << std::endl;
//		assert(false);
	}

//	this->printControllerPretty();
}

std::string InputHandler::sfmlKeyCodeToString(sf::Keyboard::Key key){
	std::string returnString;

	if(key>=sf::Keyboard::Key::A && key<=sf::Keyboard::Key::Z) {
		char character = 'A';
		character = (character + key);
		character = toupper(character);
		returnString += character;
	} else if(key>=sf::Keyboard::Key::Num0 && key<=sf::Keyboard::Key::Num9){
		int numberValue = key - sf::Keyboard::Key::Num0;
		char character = '0';
		character = (character + numberValue);
		returnString += character;
	} else if(key>=sf::Keyboard::Key::Left && key<=sf::Keyboard::Key::Down) {
		switch(key) {
			case sf::Keyboard::Key::Left : {
				returnString += "LEFT";
			} break;
			case sf::Keyboard::Key::Right : {
				returnString += "RIGHT";
			} break;
			case sf::Keyboard::Key::Up : {
				returnString += "UP";
			} break;
			case sf::Keyboard::Key::Down : {
				returnString += "DOWN";
			} break;
			default:
				break;
		}
	} else {
		returnString = "UNSUPPORTED_KEY_INPUT";
	}

	return returnString;
}

bool InputHandler::l1WasTapped() {
	return tappedL1;
}

bool InputHandler::r1WasTapped() {
	return tappedR1;
}

bool InputHandler::upWasTapped() {
	return tappedUp;
}

bool InputHandler::downWasTapped() {
	return tappedDown;
}

bool InputHandler::leftWasTapped() {
	return tappedLeft;
}

bool InputHandler::rightWasTapped() {
	return tappedRight;
}

bool InputHandler::startWasTapped() {
	return tappedStart;
}

bool InputHandler::selectWasTapped() {
	return tappedSelect;
}

bool InputHandler::button1WasTapped() {
	return tappedButton1;
}

bool InputHandler::button2WasTapped() {
	return tappedButton2;
}

bool InputHandler::button3WasTapped() {
	return tappedButton3;
}

bool InputHandler::button4WasTapped() {
	return tappedButton4;
}

bool InputHandler::l1WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedL1) {
		wasTapped = true;
		tappedL1 = false;
	}

	return wasTapped;
}

bool InputHandler::r1WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedR1) {
		wasTapped = true;
		tappedR1 = false;
	}

	return wasTapped;
}

bool InputHandler::upWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedUp) {
		wasTapped = true;
		tappedUp = false;
	}

	return wasTapped;
}

bool InputHandler::downWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedDown) {
		wasTapped = true;
		tappedDown = false;
	}

	return wasTapped;
}

bool InputHandler::leftWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedLeft) {
		wasTapped = true;
		tappedLeft = false;
	}

	return wasTapped;
}

bool InputHandler::rightWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedRight) {
		wasTapped = true;
		tappedRight = false;
	}

	return wasTapped;
}

bool InputHandler::startWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedStart) {
		wasTapped = true;
		tappedStart = false;
	}

	return wasTapped;
}

bool InputHandler::selectWasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedSelect) {
		wasTapped = true;
		tappedSelect = false;
	}

	return wasTapped;
}

bool InputHandler::button1WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedButton1) {
		wasTapped = true;
		tappedButton1 = false;
	}

	return wasTapped;
}

bool InputHandler::button2WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedButton2) {
		wasTapped = true;
		tappedButton2 = false;
	}

	return wasTapped;
}

bool InputHandler::button3WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedButton3) {
		wasTapped = true;
		tappedButton3 = false;
	}

	return wasTapped;
}

bool InputHandler::button4WasTapped_consumeInput(){
	bool wasTapped = false;
	if(tappedButton4) {
		wasTapped = true;
		tappedButton4 = false;
	}

	return wasTapped;
}

/* printControllerPretty
 *
 * Displays a pretty console output rendition of a controller.
 */
void InputHandler::printControllerPretty(){
	std::cout << std::endl << std::endl;
	std::cout << " /" << mL2Position << "\\         /" << mR2Position << "\\" << std::endl;
	std::cout << " |" << mL1 << "|_________|" << mR1 << "|" << std::endl;
	std::cout << "/ " << mUp << "           " << mButton4 << " \\" << std::endl;
	std::cout << "|" << mLeft << " " << mRight << "         " << mButton1 << " " << mButton3 << "|" << std::endl;
	std::cout << "\\ " << mDown << "   " << mSelect << "  " << mStart << "    " << mButton2 << " /" << std::endl;
	std::cout << "(" << this->getLeftJoystickPosition().x << "," << this->getLeftJoystickPosition().y << ")       (" << this->getRightJoystickPosition().x << "," << this->getRightJoystickPosition().y << ")" << std::endl;
}
