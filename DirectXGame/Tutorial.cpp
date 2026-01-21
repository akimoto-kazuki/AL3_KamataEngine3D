#include "Tutorial.h"

Tutorial::~Tutorial()
{
	delete fade_;
}

void Tutorial::Initialize()
{
	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Tutorial::Update()
{
	switch (phase_) 
	{

	case Tutorial::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Tutorial::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Tutorial::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void Tutorial::Draw()
{ fade_->Draw(); }
