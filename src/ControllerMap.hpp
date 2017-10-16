#ifndef CONTROLLERMAP_HPP_
#define CONTROLLERMAP_HPP_

#include <map>
#include <string>

#include "extlibs/rapidjson/document.h"

#include "common.hpp"

class PlayContext;

class ControllerMap {
private:
	std::string mControllerManagerPath;

	//map from standard controls to custom controls
	std::map<std::string, std::string> mCustomControlToStandardControlMap;
	rapidjson::Document* jsonDoc;

public:
	ControllerMap();
	virtual ~ControllerMap();

	void findController();

	//checks the map for the true input and returns the standard controller event
	std::string inputControlToStandardControl(std::string);

	void setInputToStandardEvent(std::string input, std::string standardEvent);
	void saveCustomLayout();

};

#endif /* CONTROLLERMAP_HPP_ */
