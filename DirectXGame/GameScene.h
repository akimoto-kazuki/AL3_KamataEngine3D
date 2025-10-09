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
#include "Shoot.h"
#include "Mouse.h"
#include "Cursor3D.h"
#include "CameraRay.h"

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

	Phase phase_;

	KamataEngine::Model* model_ = nullptr;
	
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

	std::list<Enemy*> enemies_;

	int enemySpoon = 25;

	CameraController*cameraController_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	Skydome* skydome_ = nullptr;

	MapChipField* mapChipField_;
	void GenerateBlocks();
	void ChangePhase();

	DeathParticles* deathParticles_ = nullptr;

	KamataEngine::Model* modelDeath_= nullptr;

	KamataEngine::Vector3 mousePos_;

	KamataEngine::Model* modelLockOn_ = nullptr;

private:

	uint32_t textureHandle_ = 0;
	KamataEngine::Model* modelBlock_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	int countTimer = 0;
	int countMin = 0;
	int isCountDown = false;

	DirectXCommon* dxCommon = nullptr;

	ID3D12Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	// 自前クラス
	Mouse mouse;
	CameraRay ray;
	Cursor3D* cursor = nullptr;

	// カメラ行列（仮）
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;

	


public:
	//初期化
	void Initialize();
	GameScene();
	~GameScene();
	//更新
	void Update();
	//描画
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

	const KamataEngine::Vector3& GetMousePosition() const { return mousePos_; }

	Fade* fade_ = nullptr;
};
