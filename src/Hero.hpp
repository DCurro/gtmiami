#ifndef HERO_HPP_
#define HERO_HPP_

#include "Person.hpp"
#include "DriverBody.hpp"
#include "ShooterBody.hpp"

class ActionTimer;


class Hero : public Person, public DriverBody {
protected:
	ACCESSOR(Animation*, WalkingAnimation);
	ACCESSOR(Animation*, IdleAnimation);
	ACCESSOR(Animation*, DeadAnimation);

public:
	Hero();
	virtual ~Hero();

	using Person::setup;
	virtual void setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor);
	virtual void prepareForReuse();

	virtual void update(float timeStep);
	virtual void draw(sf::RenderTarget& window);

	//<Animal>
	virtual float motionSpeed() override;
	virtual float maxHealthPoints() override;
	virtual Animation* idleAnimation() override;
	virtual Animation* walkAnimation() override;
	virtual Animation* deadAnimation() override;
    virtual void hearWeaponSoundAtLocation(b2Vec2 location) override;

	//<DriverBody>
	virtual void drive() override;
	virtual void willBeginDriving() override;
	virtual void willEndDriving() override;

	virtual void requestDropOffDriver() override;
	virtual void requestApplyBreak() override;
	virtual void requestApplyGas(float gasPower) override;
	virtual void requestTurnWheel(float turnPower) override;

	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* HERO_HPP_ */
