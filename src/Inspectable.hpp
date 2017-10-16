#ifndef INSPECTABLE_HPP_
#define INSPECTABLE_HPP_

#include <string>
#include <tuple>
#include <functional>
#include <vector>

#include "common.hpp"

class ItemDescription {
public:
	enum class UsageType {
		NO_USAGE,
		BOOL,
		INTEGER,
		FLOAT,
	};

	GETTER(std::string, Description);
	GETTER(std::function<void(bool)>, SetterMethod);
	GETTER(UsageType, UsageType);

	ItemDescription(std::string description, std::function<void(bool)> setterMethod, UsageType usagedType);
};

class Inspectable {
public:
	virtual std::string getDebugName() = 0;
	virtual std::vector<ItemDescription> getDebugDescription() = 0;
};

#endif /* INSPECTABLE_HPP_ */
