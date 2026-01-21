#include "Clear.h"

Clear::~Clear() 
{
	delete fade_; 
}

void Clear::Initialize()
{
	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Clear::Update() 
{
	switch (phase_) {

	case Clear::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Clear::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Clear::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void Clear::Draw() 
{ fade_->Draw(); }
