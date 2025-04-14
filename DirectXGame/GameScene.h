#pragma once
#include "KamataEngine.h"

class GameScene 
{
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

public:
	//初期化
	void Initialize();
	~GameScene();
	//更新
	void Update();
	//描画
	void Draw();

private:
	uint32_t textureHandle_ = 0;
};
