#include "Inspectable.hpp"

ItemDescription::ItemDescription(std::string description, std::function<void(bool)> setterMethod, UsageType usagedType)
	: mDescription(description), mSetterMethod(setterMethod), mUsageType(usagedType) {
}
