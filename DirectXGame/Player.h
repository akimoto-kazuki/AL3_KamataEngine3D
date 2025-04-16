#pragma once
#include "KamataEngine.h"
//自キャラ
class Player 
{
private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

public:
	//初期化
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
	//更新
	void Update();
	//描画
	void Draw();
};
