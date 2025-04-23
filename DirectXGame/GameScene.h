#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

class GameScene 
{

	KamataEngine::Model* model_ = nullptr;
	
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;




private:

	uint32_t textureHandle_ = 0;
	KamataEngine::Model* modelBlock_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;


public:
	//初期化
	void Initialize();
	~GameScene();
	//更新
	void Update();
	//描画
	void Draw();
};
