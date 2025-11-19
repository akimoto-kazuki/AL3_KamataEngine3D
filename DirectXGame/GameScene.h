#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include <vector>
#include "DeathParticles.h"
#include "Fade.h"
#include <time.h>
#include "LookOn.h"

class GameScene 
{

	enum class Phase
	{
		kPlay,
		kDeath,
		kFadeIn,
		kFadeOut,
		kClearFadeOut,
	};

public:

	// 初期化
	void Initialize();
	~GameScene();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 全ての当たり判定
	void CheckAllCollisions();

	// 終了
	bool finished_ = false;

	// デスフラグ
	bool IsFinished() const { return finished_; }

	// クリア
	bool clear_ = false;

	bool IsClear() const { return clear_; }

	Fade* fade_ = nullptr;

	

private:

	Phase phase_;

	// Model
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelLookOn_ = nullptr;
	KamataEngine::Model* modelDeath_ = nullptr;

	KamataEngine::Camera camera_;

	CameraController*cameraController_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	Player* player_ = nullptr;

	std::list<Enemy*> enemies_;

	int enemySpoon = 25;

	Skydome* skydome_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;

	LookOn* lookOn_ = nullptr;

	MapChipField* mapChipField_;
	void GenerateBlocks();
	void ChangePhase();

	uint32_t textureHandle_ = 0;
	KamataEngine::Model* modelBlock_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	int countTimer = 0;
	int countMin = 0;
	int isCountDown = false;

	ImGuiManager* imGuiManager = nullptr;

};
