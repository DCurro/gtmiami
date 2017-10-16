#include "TimeStepSliderViewController.hpp"

#include "PlayContext.hpp"
#include "PlayGameMode.hpp"
#include "Game.hpp"

TimeStepSliderViewController::TimeStepSliderViewController(PlayContext* playContext) : ViewController(playContext) {
}

TimeStepSliderViewController::~TimeStepSliderViewController() {
}

void TimeStepSliderViewController::setupGUI() {
	ViewController::setupGUI();

	mView->SetStyle(sfg::Window::Style::BACKGROUND);

	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL,0.0f);

	sfg::Label::Ptr fpsLabel = sfg::Label::Create("FPS");
	sfg::Separator::Ptr separator = sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL);
	sfg::Label::Ptr zeroLabel = sfg::Label::Create("60");

	float defaultTimeStep = 1.0f/60.0f;
	float timeStepIncrements = 1.0f/600.0f;
	sfg::Scale::Ptr scale = sfg::Scale::Create(0,defaultTimeStep,timeStepIncrements,sfg::Scale::Orientation::VERTICAL);
	scale->SetRequisition(sf::Vector2f(20,80));
	mAdjustment = scale->GetAdjustment();
	mAdjustment->SetValue(defaultTimeStep);
	mAdjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( std::bind( &TimeStepSliderViewController::AdjustmentChanged, this ) );

	sfg::Label::Ptr sixtyLabel = sfg::Label::Create("0");

	box->Pack(fpsLabel);
	box->Pack(separator);
	box->Pack(zeroLabel);
	box->Pack(scale);
	box->Pack(sixtyLabel);

	mView->Add(box);
}

void TimeStepSliderViewController::AdjustmentChanged() {
	mPlayContext->getPlayGameMode()->setTimeStep(mAdjustment->GetValue());
}
