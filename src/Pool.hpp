#ifndef POOL_HPP_
#define POOL_HPP_

#include <vector>
#include <iostream>
#include <typeinfo>

#include "UniqueIDManager.hpp"

class IdentificationNumberChangedNotificationReceiver {
public:
	virtual void onReceivedIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) = 0;

	virtual ~IdentificationNumberChangedNotificationReceiver(){
	}
};

class IdentificationNumberChangedNotificationSender {
public:
	virtual void sendIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) = 0;
	virtual void registerListener(IdentificationNumberChangedNotificationReceiver* object) = 0;
	virtual void unregisterListener(IdentificationNumberChangedNotificationReceiver* object) = 0;
	virtual void prepareForReuse() = 0;

	virtual ~IdentificationNumberChangedNotificationSender(){
	}
};

template <typename T>
class Pool {

	typedef typename std::remove_pointer<T>::type E;
private:
	std::vector<T> mPool;
	int mLeasedCount = 0;

public:
	T take() {
		mLeasedCount++;

		T returnObj;

		bool hasObjectsInPool = (mPool.size() > 0);
		if(hasObjectsInPool){
			returnObj = mPool.at(0);
			mPool.erase(mPool.begin());
			std::cout << "reusing a " << typeid(returnObj).name() << " from the pool" << std::endl;
		} else {
			returnObj = new E();
			unsigned int uid = UniqueIDManager::generateUniqueID();
			returnObj->setID(uid);
			std::cout << "creating a " << typeid(returnObj).name() << " from the pool" << std::endl;
		}

		return returnObj;
	}

	void giveBack(T obj) {
		mLeasedCount--;

		mPool.push_back(obj);
		unsigned int uid = UniqueIDManager::generateUniqueID();
		unsigned int oldUid = obj->getID();
		obj->setID(uid);
		obj->sendIdentificationNumberHasBecomeInvalidNotification(oldUid);
		obj->prepareForReuse();
	}

	int size(){
		return mPool.size() + mLeasedCount;
	}

	int contains(){
		return mPool.size();
	}

	int leased(){
		return mLeasedCount;
	}
};

#endif /* POOL_HPP_ */
