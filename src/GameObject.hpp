#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include <map>

#include "Pool.hpp"

class PooledPlayEntity;

class GameObject : public IdentificationNumberChangedNotificationReceiver {
public:
	GameObject();
	virtual ~GameObject();

	virtual void registerPooledPlayEntity(PooledPlayEntity* entity);

	// <IdentificationNumberChangedNotificationReceiver>
	virtual void onReceivedIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) override;

protected:
	std::map<unsigned int, PooledPlayEntity*> mPooledPlayEntityIdMap;
	//id (at the time of listening), entity
	bool isValid(PooledPlayEntity* entity);
};

#endif /* GAMEOBJECT_HPP_ */
