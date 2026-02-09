#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include <vector>
#include "Fade.h"
#include <time.h>
#include "LookOn.h"

class Player;
class Enemy;
class Time;
class GameScene 
{

	enum class Phase
	{
		kPlay,
		kFadeIn,
		kFadeOut,
	};

public:

	// 初期化
	void Initialize(Time* timer);
	// デストラクタ
	~GameScene();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 全ての当たり判定
	void CheckAllCollisions();

	void CheckPlayerBulletEnemy();

	//void CheckPlayerBulletBreakBlock();

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

	Player* player_ = nullptr;
	Time* timer_ = nullptr;
	CameraController* cameraController_ = nullptr;
	Skydome* skydome_ = nullptr;
	LookOn* lookOn_ = nullptr;
	MapChipField* mapChipField_;
	std::list<Enemy*> enemies_;

	// Model
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelShot_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Camera camera_;


	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	int enemySpoon = 25;

	void GenerateBlocks();

	uint32_t textureHandle_ = 0;
	KamataEngine::Model* modelBlock_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	ImGuiManager* imGuiManager = nullptr;

	bool isCountTimer_ = true;

	int target_; // 的の数
	const int kMaxTarget_ = 4; // 的の最大数

};
