#ifndef POOLEDPLAYENTITY_HPP_
#define POOLEDPLAYENTITY_HPP_

#include "PlayEntity.hpp"
#include "Pool.hpp"
#include "common.hpp"

class PooledPlayEntity : public PlayEntity, public IdentificationNumberChangedNotificationSender, public Inspectable {
typedef std::pair<IdentificationNumberChangedNotificationReceiver*, IdentificationNumberChangedNotificationReceiver*> selfPair;
typedef std::map<IdentificationNumberChangedNotificationReceiver*, IdentificationNumberChangedNotificationReceiver*> GameObjectSelfReferentialMap;

public:
	PooledPlayEntity();
	virtual ~PooledPlayEntity();
    
    GETTER(int, PersistentIdNumber);
    
	virtual void sendIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) override;
	virtual void registerListener(IdentificationNumberChangedNotificationReceiver* object) override;
	virtual void unregisterListener(IdentificationNumberChangedNotificationReceiver* object) override;
	virtual void prepareForReuse() override;
    
    using PlayEntity::setup;
    virtual void setup(PlayContext* playContext, int persistentIdNumber, int floor);
    
    virtual void update(float timeStep);

protected:
	ACCESSOR(GameObjectSelfReferentialMap , Listeners);
};

#endif /* POOLEDPLAYENTITY_HPP_ */
