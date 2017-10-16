#include "ObjectInspectorViewController.hpp"

#include <math.h>

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "PooledPlayEntity.hpp"
#include "Game.hpp"
#include "DistrictManager.hpp"

#define THREE_DEGREES_IN_RADS 3.0*DEGREES_TO_RADIANS

float normalizeRadianAngle(const float radianAngle) {
	float rotation = std::fmod(radianAngle, 2.0*PI_CONSTANT);
	if(rotation < 0){
		rotation = 2.0*PI_CONSTANT + rotation;
	}

	return rotation;
}

ObjectInspectorViewController::ObjectInspectorViewController(PlayContext* playContext) : ViewController(playContext) {
	inspectingEntity = NULL;
}

ObjectInspectorViewController::~ObjectInspectorViewController() {
}

void ObjectInspectorViewController::setupGUI() {
	ViewController::setupGUI();

	mView->SetStyle(sfg::Window::Style::BACKGROUND);

	sfg::Label::Ptr titleLabel = sfg::Label::Create("Inspector");

	sfg::Box::Ptr canvasBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.0f);
	mComboBox = sfg::ComboBox::Create();
    mComboBox->GetSignal(sfg::ComboBox::OnLeftClick).Connect( std::bind(&ObjectInspectorViewController::onComboBoxOpened, this) );
    mComboBox->GetSignal(sfg::ComboBox::OnOpen).Connect( std::bind(&ObjectInspectorViewController::onComboBoxOpened, this) );
	mComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect( std::bind(&ObjectInspectorViewController::onComboBoxItemSelected, this) );

	mCanvas = sfg::Canvas::Create();
	mCanvas->SetRequisition(sf::Vector2f(INSPECTOR_DRAW_AREA_WIDTH,INSPECTOR_DRAW_AREA_HEIGHT));
	mCanvas->Clear(sf::Color::White, false);
	mCanvas->Display();
	canvasBox->Pack(mComboBox);
	canvasBox->Pack(mCanvas);

	sfg::Box::Ptr rotationContainer = sfg::Box::Create(sfg::Box::Orientation::VERTICAL,0.0f);
	sfg::Label::Ptr rotationLabel = sfg::Label::Create("rotation:");
	sfg::Box::Ptr rotationBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL,0.0f);
	sfg::Label::Ptr zeroLabel = sfg::Label::Create(" 0 ");
	sfg::Scale::Ptr scale = sfg::Scale::Create(0,2*PI_CONSTANT,THREE_DEGREES_IN_RADS,sfg::Scale::Orientation::HORIZONTAL);
	scale->SetRequisition(sf::Vector2f(INSPECTOR_DRAW_AREA_WIDTH/2.0f,20));
	mRotationAdjustment = scale->GetAdjustment();

	mRotationAdjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( std::bind(&ObjectInspectorViewController::rotationScaleChanged, this) );
	sfg::Label::Ptr threeSixtyLabel = sfg::Label::Create("360");
	rotationBox->Pack(zeroLabel);
	rotationBox->Pack(scale);
	rotationBox->Pack(threeSixtyLabel);
	rotationContainer->Pack(rotationLabel);
	rotationContainer->Pack(rotationBox);

	sfg::Box::Ptr positionContainer = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.0f);
	sfg::Label::Ptr positionLabel = sfg::Label::Create("position:");
	sfg::Box::Ptr xBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sfg::Label::Ptr xLabel = sfg::Label::Create("x:");
	sfg::SpinButton::Ptr xSpinner = sfg::SpinButton::Create(-99999.99f,99999.99f,0.1f);
	xSpinner->SetRequisition(sf::Vector2f(INSPECTOR_DRAW_AREA_WIDTH-30,20));
	xSpinner->GetSignal( sfg::SpinButton::OnValueChanged ).Connect( std::bind( &ObjectInspectorViewController::PositionXSpinnerChanged, this ) );
	mXPositionAdjustment = xSpinner->GetAdjustment();
	xSpinner->SetDigits( 2 );
	xBox->Pack(xLabel);
	xBox->Pack(xSpinner);
	sfg::Box::Ptr yBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sfg::Label::Ptr yLabel = sfg::Label::Create("y:");
	sfg::SpinButton::Ptr ySpinner = sfg::SpinButton::Create(-99999.99f,99999.99f,-0.1f);
	ySpinner->SetRequisition(sf::Vector2f(INSPECTOR_DRAW_AREA_WIDTH-30,20));
	ySpinner->GetSignal( sfg::SpinButton::OnValueChanged ).Connect( std::bind( &ObjectInspectorViewController::PositionYSpinnerChanged, this ) );
	mYPositionAdjustment = ySpinner->GetAdjustment();
	ySpinner->SetDigits( 2 );
	yBox->Pack(yLabel);
	yBox->Pack(ySpinner);
	positionContainer->Pack(positionLabel);
	positionContainer->Pack(xBox);
	positionContainer->Pack(yBox);
    auto deleteObjectButton = sfg::Button::Create("delete");
    positionContainer->Pack(deleteObjectButton);
    deleteObjectButton->GetSignal(sfg::Button::OnLeftClick).Connect( std::bind(&ObjectInspectorViewController::DeleteButtonPressed, this));
    

	sfg::Separator::Ptr separator = sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL);

	sfg::Box::Ptr debugInfoContainer = sfg::Box::Create(sfg::Box::Orientation::VERTICAL,0.0f);
	sfg::Label::Ptr debugInfoLabelHeader = sfg::Label::Create("debug info:");
	mDebugInfoLabel = sfg::Label::Create("");
	mDebugInfoLabel->SetRequisition( sf::Vector2f( INSPECTOR_DRAW_AREA_WIDTH, 0.0f ) );
	mDebugInfoLabel->SetLineWrap( true );
	debugInfoContainer->Pack(debugInfoLabelHeader);
	debugInfoContainer->Pack(mDebugInfoLabel);


	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
	box->Pack(titleLabel);
	box->Pack(canvasBox);
	box->Pack(rotationContainer);
	box->Pack(positionContainer);
	box->Pack(separator);
	box->Pack(debugInfoContainer);

	mView->Add(box);
}

void ObjectInspectorViewController::handleEvent(sf::Event event) {
	switch(event.type) {
		case sf::Event::MouseButtonPressed : {
			if(event.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2f mouseAbsoluteScreenPosition(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2f mouseWorldPosition = mPlayContext->getCamera()->screenToWorld_relative(mouseAbsoluteScreenPosition);
				b2Vec2 mouseWorldPositionB2(mouseWorldPosition.x, mouseWorldPosition.y);

				b2AABB aabb;
				aabb.lowerBound = mouseWorldPositionB2;
				aabb.upperBound = mouseWorldPositionB2;

				for(auto entity : inspectableEntities){
					mComboBox->RemoveItem(0);
					entity->unregisterListener(this);
				}

				mQueryCallback.clearBodies();
				inspectableEntities.clear();

				mQueryCallback.setTouchPoint(mouseWorldPositionB2);
				mPlayContext->getWorld()->QueryAABB(&mQueryCallback,aabb);
				if(mQueryCallback.getBodies().size() > 0) {
					for(auto body : mQueryCallback.getBodies()){
						if(body->GetUserData() == NULL){
							continue;
						}

						PlayEntity* playEntity = (PlayEntity*)body->GetUserData();
						PooledPlayEntity* pooledEntity = dynamic_cast<PooledPlayEntity*>( playEntity );
						Inspectable* inspectableEntity = dynamic_cast<Inspectable*>( pooledEntity );
						bool alreadyContainsEntity = (std::find(inspectableEntities.begin(), inspectableEntities.end(), inspectableEntity) != inspectableEntities.end());
						if(pooledEntity != NULL && inspectableEntity != NULL && !alreadyContainsEntity) {
							registerPooledPlayEntity(pooledEntity);
							inspectableEntities.push_back(pooledEntity);
							pooledEntity->registerListener(this);
							std::string name = pooledEntity->getDebugName();
							mComboBox->AppendItem(name);
						}
					}

					if(inspectableEntities.size() > 0) {
						mComboBox->SelectItem(0);
						inspectingEntity =  inspectableEntities.at(0);
						mRotationAdjustment->SetValue( normalizeRadianAngle(inspectingEntity->getBody()->GetAngle()) );
						mXPositionAdjustment->SetValue(inspectingEntity->getBody()->GetPosition().x);
						mYPositionAdjustment->SetValue(inspectingEntity->getBody()->GetPosition().y);
					}
				} else {
					inspectingEntity = NULL;
					mComboBox->Refresh();
					mCanvas->Clear(sf::Color::White, false);
					mDebugInfoLabel->SetText("");
					mCanvas->Display();
				}
			}
		} break;

		default:
			break;
	}
}

void ObjectInspectorViewController::update(float timestep) {
	ViewController::update(timestep);

	if( isValid(inspectingEntity) ) {
		float rotation = normalizeRadianAngle(inspectingEntity->getBody()->GetAngle());
		float x = inspectingEntity->getBody()->GetPosition().x;
		float y = inspectingEntity->getBody()->GetPosition().y;

		mRotationAdjustment->SetValue(rotation);
		mXPositionAdjustment->SetValue(x);
		mYPositionAdjustment->SetValue(y);

		sf::Sprite sprite(inspectingEntity->getSprite());
		sprite.setPosition(INSPECTOR_DRAW_AREA_WIDTH/2.0f,INSPECTOR_DRAW_AREA_HEIGHT/2.0f);
		mCanvas->Clear(sf::Color::White, false);
		mCanvas->Draw(sprite);
		mCanvas->Display();

		std::vector<ItemDescription> itemDescriptions = inspectingEntity->getDebugDescription();
		for(ItemDescription item : itemDescriptions){
			mDebugInfoLabel->SetText(item.getDescription());
		}
	} else {
		mCanvas->Clear(sf::Color::White, false);
		mCanvas->Draw(sprite);
		mCanvas->Display();
	}
}

void ObjectInspectorViewController::rotationScaleChanged() {
	if( isValid(inspectingEntity) ) {
		b2Vec2 position = inspectingEntity->getBody()->GetPosition();
		inspectingEntity->getBody()->SetTransform(position, normalizeRadianAngle(mRotationAdjustment->GetValue()));
	}
}

void ObjectInspectorViewController::PositionXSpinnerChanged() {
	if( isValid(inspectingEntity) ) {
		b2Vec2 position(mXPositionAdjustment->GetValue(), inspectingEntity->getBody()->GetPosition().y);
		float angle = inspectingEntity->getBody()->GetAngle();
		inspectingEntity->getBody()->SetTransform(position, angle);
	}
}

void ObjectInspectorViewController::PositionYSpinnerChanged() {
	if( isValid(inspectingEntity) ) {
		b2Vec2 position(inspectingEntity->getBody()->GetPosition().x, mYPositionAdjustment->GetValue());
		float angle = inspectingEntity->getBody()->GetAngle();
		inspectingEntity->getBody()->SetTransform(position, angle);
	}
}

void ObjectInspectorViewController::onComboBoxItemSelected() {
    this->setClicked(true);
    
	int selectIndex = mComboBox->GetSelectedItem();
	inspectingEntity = inspectableEntities.at(selectIndex);
}

void ObjectInspectorViewController::onComboBoxOpened() {
    this->setClicked(true);
}

void ObjectInspectorViewController::DeleteButtonPressed() {
    this->setClicked(true);
    
    if( isValid(inspectingEntity) ) {
        this->mPlayContext->getDistrictManager()->unsetPooledPlayEntityForPersistenceIdentifer(inspectingEntity->getPersistentIdNumber());
        inspectingEntity->setIsStillPartOfGame(false);
    }
}

// <IdentificaitonNumberChangedReceiver>
void ObjectInspectorViewController::onReceivedIdentificationNumberHasBecomeInvalidNotification(unsigned int oldID) {
	GameObject::onReceivedIdentificationNumberHasBecomeInvalidNotification(oldID);

	for(PooledPlayEntity* entity : inspectableEntities) {
		if(entity->getID() == oldID) {
			mComboBox->RemoveItem(0);
			inspectableEntities.erase(std::remove(inspectableEntities.begin(), inspectableEntities.end(), entity), inspectableEntities.end());
			break;
		}
	}
}
