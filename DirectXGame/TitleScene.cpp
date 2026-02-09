#include "TitleScene.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

void TitleScene::Initialize() 
{ 
	model_ = Model::CreateFromOBJ("titleFont",true);
	modelPlayer_ = Model::CreateFromOBJ("titleSpace",true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {5, 5, 5};
	worldTransform_.translation_ = {0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {5, 5, 5};
	worldTransformPlayer_.translation_ = {0, -5, 0};

	fade_ = new Fade();
	fade_->Initialize();

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);


	fade_->Start(Fade::Status::FadeIn, 1.0f);

}

TitleScene::~TitleScene()
{
	delete model_;
	delete modelPlayer_;
	delete fade_;
}

void TitleScene::Update()
{
	skydome_->Update();
	switch (phase_) 
	{
	case TitleScene::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE))
		{
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case TitleScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished())
		{
			phase_ = Phase::kMain;
		}
		break;
	case TitleScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	worldTransformPlayer_.TransferMatrix();
}

void TitleScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance(); 

	Model::PreDraw(dxCommon->GetCommandList());
	skydome_->Draw();
	model_->Draw(worldTransform_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	Model::PostDraw();

	fade_->Draw();

}
