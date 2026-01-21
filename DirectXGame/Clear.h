#pragma once

#include "Fade.h"
#include "KamataEngine.h"

class Clear 
{
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	void Initialize();

	~Clear();

	void Update();

	void Draw();

	// デスフラグ
	bool IsFinished() const { return finished_; }

private:
	// 終了
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
