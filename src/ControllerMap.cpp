#include "ControllerMap.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Window/Joystick.hpp>
#include "extlibs/rapidjson/allocators.h"
#include "extlibs/rapidjson/encodings.h"
#include "extlibs/rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output
#include "extlibs/rapidjson/prettywriter.h"	// for stringify JSON
#include "extlibs/rapidjson/stringbuffer.h"

#include "OS.hpp"

std::string getCustomControlForKeyInJson(rapidjson::Document* jsonDoc, std::string key) {
	if(jsonDoc->FindMember(key.c_str()) != jsonDoc->MemberEnd()){
		return jsonDoc->FindMember(key.c_str())->value.GetString();
	}
	return "";
}

void ControllerMap::findController() {
	if(this->jsonDoc){
		delete jsonDoc;
	}

	sf::Joystick::update();

	std::string vendorIDString = "key";
	std::string productIDString = "board";

	std::ostringstream convertVendorID;
	std::ostringstream convertProductID;

	for(int i=0; i<sf::Joystick::Count; i++){
		if(sf::Joystick::isConnected(i)){
			convertVendorID << sf::Joystick::getIdentification(i).vendorId;
			vendorIDString = "" + convertVendorID.str();
			convertProductID << sf::Joystick::getIdentification(i).productId;
			productIDString = "" + convertProductID.str();
			break;
		}
	}

	mControllerManagerPath = OS::getPwd() + "resources/" + "controllerMap_" + vendorIDString + "_" + productIDString + ".json";

	std::cout << "Using controller: " << vendorIDString << "-" << productIDString << std::endl;

	std::string jsonString = "{\n}"; //empty json
	std::ifstream in(mControllerManagerPath, std::ios::in | std::ios::binary);
	if(in.good()){
		jsonString = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}
	rapidjson::Document* jsonDoc = new rapidjson::Document();
	jsonDoc->Parse(jsonString.c_str());
	this->jsonDoc = jsonDoc;

	this->mCustomControlToStandardControlMap["BUTTON0"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON0");
	this->mCustomControlToStandardControlMap["BUTTON1"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON1");
	this->mCustomControlToStandardControlMap["BUTTON2"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON2");
	this->mCustomControlToStandardControlMap["BUTTON3"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON3");
	this->mCustomControlToStandardControlMap["BUTTON4"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON4");
	this->mCustomControlToStandardControlMap["BUTTON5"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON5");
	this->mCustomControlToStandardControlMap["BUTTON6"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON6");
	this->mCustomControlToStandardControlMap["BUTTON7"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON7");
	this->mCustomControlToStandardControlMap["BUTTON8"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON8");
	this->mCustomControlToStandardControlMap["BUTTON9"] =  getCustomControlForKeyInJson(this->jsonDoc, "BUTTON9");
	this->mCustomControlToStandardControlMap["BUTTON10"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON10");
	this->mCustomControlToStandardControlMap["BUTTON11"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON11");
	this->mCustomControlToStandardControlMap["BUTTON12"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON12");
	this->mCustomControlToStandardControlMap["BUTTON13"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON13");
	this->mCustomControlToStandardControlMap["BUTTON14"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON14");
	this->mCustomControlToStandardControlMap["BUTTON15"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON15");
	this->mCustomControlToStandardControlMap["BUTTON16"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON16");
	this->mCustomControlToStandardControlMap["BUTTON17"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON17");
	this->mCustomControlToStandardControlMap["BUTTON18"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON18");
	this->mCustomControlToStandardControlMap["BUTTON19"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON19");
	this->mCustomControlToStandardControlMap["BUTTON20"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON20");
	this->mCustomControlToStandardControlMap["BUTTON21"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON21");
	this->mCustomControlToStandardControlMap["BUTTON22"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON22");
	this->mCustomControlToStandardControlMap["BUTTON23"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON23");
	this->mCustomControlToStandardControlMap["BUTTON24"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON24");
	this->mCustomControlToStandardControlMap["BUTTON25"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON25");
	this->mCustomControlToStandardControlMap["BUTTON26"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON26");
	this->mCustomControlToStandardControlMap["BUTTON27"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON27");
	this->mCustomControlToStandardControlMap["BUTTON28"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON28");
	this->mCustomControlToStandardControlMap["BUTTON29"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON29");
	this->mCustomControlToStandardControlMap["BUTTON30"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON30");
	this->mCustomControlToStandardControlMap["BUTTON31"] = getCustomControlForKeyInJson(this->jsonDoc, "BUTTON31");

	this->mCustomControlToStandardControlMap["X_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "X_AXIS+");
	this->mCustomControlToStandardControlMap["X_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "X_AXIS-");
	this->mCustomControlToStandardControlMap["Y_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "Y_AXIS+");
	this->mCustomControlToStandardControlMap["Y_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "Y_AXIS-");
	this->mCustomControlToStandardControlMap["Z_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "Z_AXIS+");
	this->mCustomControlToStandardControlMap["Z_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "Z_AXIS-");
	this->mCustomControlToStandardControlMap["R_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "R_AXIS+");
	this->mCustomControlToStandardControlMap["R_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "R_AXIS-");
	this->mCustomControlToStandardControlMap["U_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "U_AXIS+");
	this->mCustomControlToStandardControlMap["U_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "U_AXIS-");
	this->mCustomControlToStandardControlMap["V_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "V_AXIS+");
	this->mCustomControlToStandardControlMap["V_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "V_AXIS-");
	this->mCustomControlToStandardControlMap["POVX_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "POVX_AXIS+");
	this->mCustomControlToStandardControlMap["POVX_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "POVX_AXIS-");
	this->mCustomControlToStandardControlMap["POVY_AXIS+"] = getCustomControlForKeyInJson(this->jsonDoc, "POVY_AXIS+");
	this->mCustomControlToStandardControlMap["POVY_AXIS-"] = getCustomControlForKeyInJson(this->jsonDoc, "POVY_AXIS-");

	this->mCustomControlToStandardControlMap["UP"] = getCustomControlForKeyInJson(this->jsonDoc, "UP");
	this->mCustomControlToStandardControlMap["DOWN"] = getCustomControlForKeyInJson(this->jsonDoc, "DOWN");
	this->mCustomControlToStandardControlMap["LEFT"] = getCustomControlForKeyInJson(this->jsonDoc, "LEFT");
	this->mCustomControlToStandardControlMap["RIGHT"] = getCustomControlForKeyInJson(this->jsonDoc, "RIGHT");

	this->mCustomControlToStandardControlMap["A"] = getCustomControlForKeyInJson(this->jsonDoc, "A");
	this->mCustomControlToStandardControlMap["B"] = getCustomControlForKeyInJson(this->jsonDoc, "B");
	this->mCustomControlToStandardControlMap["C"] = getCustomControlForKeyInJson(this->jsonDoc, "C");
	this->mCustomControlToStandardControlMap["D"] = getCustomControlForKeyInJson(this->jsonDoc, "D");
	this->mCustomControlToStandardControlMap["E"] = getCustomControlForKeyInJson(this->jsonDoc, "E");
	this->mCustomControlToStandardControlMap["F"] = getCustomControlForKeyInJson(this->jsonDoc, "F");
	this->mCustomControlToStandardControlMap["G"] = getCustomControlForKeyInJson(this->jsonDoc, "G");
	this->mCustomControlToStandardControlMap["H"] = getCustomControlForKeyInJson(this->jsonDoc, "H");
	this->mCustomControlToStandardControlMap["I"] = getCustomControlForKeyInJson(this->jsonDoc, "I");
	this->mCustomControlToStandardControlMap["J"] = getCustomControlForKeyInJson(this->jsonDoc, "J");
	this->mCustomControlToStandardControlMap["K"] = getCustomControlForKeyInJson(this->jsonDoc, "K");
	this->mCustomControlToStandardControlMap["L"] = getCustomControlForKeyInJson(this->jsonDoc, "L");
	this->mCustomControlToStandardControlMap["M"] = getCustomControlForKeyInJson(this->jsonDoc, "M");
	this->mCustomControlToStandardControlMap["N"] = getCustomControlForKeyInJson(this->jsonDoc, "N");
	this->mCustomControlToStandardControlMap["O"] = getCustomControlForKeyInJson(this->jsonDoc, "O");
	this->mCustomControlToStandardControlMap["P"] = getCustomControlForKeyInJson(this->jsonDoc, "P");
	this->mCustomControlToStandardControlMap["Q"] = getCustomControlForKeyInJson(this->jsonDoc, "Q");
	this->mCustomControlToStandardControlMap["R"] = getCustomControlForKeyInJson(this->jsonDoc, "R");
	this->mCustomControlToStandardControlMap["S"] = getCustomControlForKeyInJson(this->jsonDoc, "S");
	this->mCustomControlToStandardControlMap["T"] = getCustomControlForKeyInJson(this->jsonDoc, "T");
	this->mCustomControlToStandardControlMap["U"] = getCustomControlForKeyInJson(this->jsonDoc, "U");
	this->mCustomControlToStandardControlMap["V"] = getCustomControlForKeyInJson(this->jsonDoc, "V");
	this->mCustomControlToStandardControlMap["W"] = getCustomControlForKeyInJson(this->jsonDoc, "W");
	this->mCustomControlToStandardControlMap["X"] = getCustomControlForKeyInJson(this->jsonDoc, "X");
	this->mCustomControlToStandardControlMap["Y"] = getCustomControlForKeyInJson(this->jsonDoc, "Y");
	this->mCustomControlToStandardControlMap["Z"] = getCustomControlForKeyInJson(this->jsonDoc, "Z");

	this->mCustomControlToStandardControlMap["0"] = getCustomControlForKeyInJson(this->jsonDoc, "0");
	this->mCustomControlToStandardControlMap["1"] = getCustomControlForKeyInJson(this->jsonDoc, "1");
	this->mCustomControlToStandardControlMap["2"] = getCustomControlForKeyInJson(this->jsonDoc, "2");
	this->mCustomControlToStandardControlMap["3"] = getCustomControlForKeyInJson(this->jsonDoc, "3");
	this->mCustomControlToStandardControlMap["4"] = getCustomControlForKeyInJson(this->jsonDoc, "4");
	this->mCustomControlToStandardControlMap["5"] = getCustomControlForKeyInJson(this->jsonDoc, "5");
	this->mCustomControlToStandardControlMap["6"] = getCustomControlForKeyInJson(this->jsonDoc, "6");
	this->mCustomControlToStandardControlMap["7"] = getCustomControlForKeyInJson(this->jsonDoc, "7");
	this->mCustomControlToStandardControlMap["8"] = getCustomControlForKeyInJson(this->jsonDoc, "8");
	this->mCustomControlToStandardControlMap["9"] = getCustomControlForKeyInJson(this->jsonDoc, "9");
}

ControllerMap::ControllerMap(){
	this->jsonDoc = NULL;
	this->findController();
}

ControllerMap::~ControllerMap() {
	this->saveCustomLayout();
}


//Given a custom control, we want the standard control
std::string ControllerMap::inputControlToStandardControl(std::string input) {
	return this->mCustomControlToStandardControlMap[input];
}

void ControllerMap::setInputToStandardEvent(std::string input, std::string standardEvent){
	this->mCustomControlToStandardControlMap[input] = standardEvent;
	this->jsonDoc->FindMember(input.c_str())->value.SetString(standardEvent.c_str(), this->jsonDoc->GetAllocator());
}

void ControllerMap::saveCustomLayout(){
	char cbuf[1024]; rapidjson::MemoryPoolAllocator<> allocator (cbuf, sizeof cbuf);
	rapidjson::Document meta (&allocator, 256);

	for(auto iter=mCustomControlToStandardControlMap.begin(); iter!=mCustomControlToStandardControlMap.end(); iter++){
		std::string key = iter->first;
		std::string value = iter->second;
		if(jsonDoc->FindMember(key.c_str()) != jsonDoc->MemberEnd()){
			this->jsonDoc->FindMember(key.c_str())->value.SetString(value.c_str(), this->jsonDoc->GetAllocator());
		}
	}

	typedef rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> StringBuffer;
	StringBuffer buf (&allocator);
	rapidjson::PrettyWriter<StringBuffer> writer (buf, &allocator);
	this->jsonDoc->Accept (writer);
	std::string json (buf.GetString(), buf.GetSize());

	std::ofstream of(mControllerManagerPath);
	of << json;
	if (!of.good()) throw std::runtime_error ("Can't write the JSON string to the file!");
	of.flush();
}
