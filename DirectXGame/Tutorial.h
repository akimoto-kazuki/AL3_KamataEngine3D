#pragma once

#include "Fade.h"
#include "KamataEngine.h"
#include "Skydome.h"

class Tutorial 
{
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	void Initialize();

	~Tutorial();

	void Update();

	void Draw();

	// デスフラグ
	bool IsFinished() const { return finished_; }

private:

	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// 終了
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
