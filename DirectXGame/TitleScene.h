#pragma once
#include "KamataEngine.h"

class TitleScene 
{
public:

	void Initialize();

	~TitleScene();

	void Update();

	void Draw();
	

	// デスフラグ
	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	float rotate = 0.0f;

	// 終了
	bool finished_ = false;

};