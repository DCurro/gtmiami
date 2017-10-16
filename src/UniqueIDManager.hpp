#ifndef UNIQUEIDMANAGER_HPP_
#define UNIQUEIDMANAGER_HPP_

class UniqueIDManager {
private:
	UniqueIDManager();
	virtual ~UniqueIDManager();

	static unsigned int nextID;

public:
	static unsigned int generateUniqueID();
};

#endif /* UNIQUEIDMANAGER_HPP_ */
