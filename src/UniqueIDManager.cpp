#include "UniqueIDManager.hpp"

unsigned int UniqueIDManager::nextID = 0;

UniqueIDManager::UniqueIDManager() {
}

UniqueIDManager::~UniqueIDManager() {
}

unsigned int UniqueIDManager::generateUniqueID() {
	nextID++;
	return nextID;
}
