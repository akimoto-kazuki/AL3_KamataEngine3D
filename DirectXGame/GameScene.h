#pragma once
#include "KamataEngine.h"
#include "Player.h"

class GameScene 
{

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

private:
	uint32_t textureHandle_ = 0;

public:
	//初期化
	void Initialize();
	~GameScene();
	//更新
	void Update();
	//描画
	void Draw();
};
