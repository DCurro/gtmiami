#ifndef OBJECTINSPECTORVIEWCONTROLLER_HPP_
#define OBJECTINSPECTORVIEWCONTROLLER_HPP_

#include <vector>
#include <set>

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <Box2D/Box2D.h>

#include "ViewController.hpp"
#include "Pool.hpp"
#include "PooledPlayEntity.hpp"

class PlayContext;
class PooledPlayEntity;

class ObjectInspectorViewController : public ViewController {
private:
	class InspectorQueryCallback : public b2QueryCallback {
		private:
			b2Vec2 mTouchPoint;
			std::vector<b2Body*> foundBodies;
			std::set<int> ids;
		public:
			void setTouchPoint(b2Vec2 touchPoint) {
				this->mTouchPoint = touchPoint;
			}

			std::vector<b2Body*> getBodies() {
				return foundBodies;
			}
			std::set<int> getIDs(){
				return ids;
			}
			void clearBodies() {
				foundBodies.clear();
			}
			bool ReportFixture(b2Fixture* fixture) {
				if(fixture->GetBody()->GetUserData() != NULL){
					if(std::find(foundBodies.begin(), foundBodies.end(), fixture->GetBody()) == foundBodies.end()) {
						if(fixture->TestPoint(mTouchPoint)){
							foundBodies.push_back( fixture->GetBody() );
						}
					}
					ids.insert( static_cast<PooledPlayEntity*>(fixture->GetBody()->GetUserData())->getID() );
				}
				return true;
			}
	};

protected:
	sfg::ComboBox::Ptr mComboBox;
	sfg::Canvas::Ptr mCanvas;
	sfg::Adjustment::Ptr mRotationAdjustment;
	sfg::Adjustment::Ptr mXPositionAdjustment;
	sfg::Adjustment::Ptr mYPositionAdjustment;
	sfg::Label::Ptr mDebugInfoLabel;
	sf::Sprite sprite;

	void rotationScaleChanged();
	void PositionXSpinnerChanged();
	void PositionYSpinnerChanged();
	void onComboBoxItemSelected();
    void onComboBoxOpened();
    void DeleteButtonPressed();

public:
	static const int INSPECTOR_DRAW_AREA_WIDTH = 200;
	static const int INSPECTOR_DRAW_AREA_HEIGHT = 200;

	ObjectInspectorViewController(PlayContext* playContext);
	virtual ~ObjectInspectorViewController();

	virtual void setupGUI();

	virtual void handleEvent(sf::Event event);
	virtual void update(float timestep);

	// <IdentificationNumberChangedNotificationReceiver>
	virtual void onReceivedIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) override;

private:
	InspectorQueryCallback mQueryCallback;
	std::vector<PooledPlayEntity*> inspectableEntities;
	PooledPlayEntity* inspectingEntity;
};

#endif /* OBJECTINSPECTORVIEWCONTROLLER_HPP_ */
