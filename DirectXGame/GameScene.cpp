#include "GameScene.h"
#include "MyMath.h"
#include <algorithm>
#include <cassert>
#include "Player.h"
#include "Enemy.h"
#include "Time.h"

using namespace KamataEngine;

void GameScene::Initialize(Time* timer)
{
	
	model_ = Model::Create();
	camera_.Initialize();

	phase_ = Phase::kFadeIn;

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	//Model
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelShot_ = Model::CreateFromOBJ("bullet", true);
	// ブロック
	modelBlock_ = Model::CreateFromOBJ("block", true);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	timer_ = timer;
	timer_->SetScoreDisplayMode(Time::DisplayMode::Normal);

	TextureManager::Load("lookOn.png");

	// 自キャラの生成

	lookOn_ = new LookOn();

	// カメラ
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	CameraController::Rect cameraArea = {12.0f, (100 - 12.0f), 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	
	Vector3 lookOnPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// 照準オブジェクトの初期化
	player_->SetLookOn(lookOn_);
	lookOn_->Initialize(&camera_, lookOnPosition);

	//デバッグ
	debugCamera_ = new DebugCamera(1280, 720);

	imGuiManager = ImGuiManager::GetInstance();

	target_ = kMaxTarget_;
	
}

void GameScene::GenerateBlocks() 
{
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) 
	{
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t y = 0; y < numBlockVirtical; ++y) 
	{
		for (uint32_t x = 0; x < numBlockHorizontal; ++x)
		{
			MapChipType mapChipType;
			uint8_t subID;
			mapChipType = mapChipField_->GetMapChipTypeByIndex(x, y);
			WorldTransform* worldTransform = nullptr;
			// プレイヤー
			Vector3 playerPos;
			Vector3 enemyPos;

			switch (mapChipType) 
			{
			case MapChipType::kBlank:
				break;
			case MapChipType::kBlock: 
			{
				worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[y][x] = worldTransform;
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
				break;
			case MapChipType::kPlayer:
			{
				assert(player_ == nullptr && "自キャラを2重に配置しようとしています");
				// プレイヤー
				player_ = new Player();
				// リスポーン地点
				playerPos = mapChipField_->GetMapChipPositionByIndex(x, y);
				// 初期化の置き場
				player_->Initialize(modelPlayer_, modelShot_, &camera_, playerPos);
				// マップチップ読み込み
				player_->SetMapChipField(mapChipField_);
			}
				break;
			case MapChipType::kEnemy:
			{
				subID = mapChipField_->GetMapChipSubIDByIndex(x, y);
				switch (subID)
				{
				case 0:
					Enemy* newEnemy = new Enemy();
					enemyPos = mapChipField_->GetMapChipPositionByIndex(x,y);
					newEnemy->Initialize(modelEnemy_, &camera_, enemyPos);

					enemies_.push_back(newEnemy);
					break;
				
				}
			}
				break;
			}
		}
	}
}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
	for (Enemy* enemy:enemies_) {
		delete enemy;
	}
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine:worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController_;
	delete fade_;
	delete lookOn_;
}

void GameScene::Update()
{
	
	fade_->Update();

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	switch (phase_) {

	case GameScene::Phase::kPlay:
		CheckAllCollisions();
		if (isCountTimer_) 
		{
			timer_->Update();
		}
		player_->Update();
		lookOn_->Update();
		if (target_<= 0)
		{
			phase_ = Phase::kFadeOut;
			isCountTimer_ = false;
			timer_->TimerKeep();
		}
		
		break;
	case GameScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kPlay;
			
		}
		break;
	case GameScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			finished_ =  true;
			
			timer_->FileWrite();
		}
		break;

	}
	
	skydome_->Update();
	for (Enemy* enemy : enemies_) 
	{
		enemy->Update();
		if (enemy->IsDead())
		{
			target_ -= 1;
			break;
		}
	}
	cameraController_->Update();

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	imGuiManager->Begin();

	ImGui::Begin("mouse");
	ImGui::SameLine();
	ImGui::Text("mousePosX = %f,PosY = %f", lookOn_->GetMousePosX(),lookOn_->GetMousePosY());
	ImGui::Text("mousePosX = %f,PosY = %f,PosZ = %f", lookOn_->GetWorldPosition().x, lookOn_->GetWorldPosition().y, lookOn_->GetWorldPosition().z);
	ImGui::End();

	imGuiManager->End();
#endif // DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}
	
}

void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn) 
	{
		player_->Draw();
	}
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	skydome_->Draw();

	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	lookOn_->DrawUI();
	timer_->Draw();

	Sprite::PostDraw();

	
	imGuiManager->Draw();
	

	fade_->Draw();
	
}

void GameScene::CheckPlayerBulletEnemy()
{ 
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullets();

	// 判定対象1と2の座標
	AABB aabb1, aabb2;
	for (Enemy* enemy : enemies_) {
		aabb1 = enemy->GetAABB();
		for (PlayerBullet* playerBullet : playerBullets) {
			aabb2 = playerBullet->GetAABB();

			if (IsCollision(aabb1, aabb2)) {
				enemy->OnCollision(playerBullet);

				playerBullet->OnCollision(enemy);
			}
		}
	}
}

void GameScene::CheckAllCollisions()
{

	// プレイヤーの弾が敵に当たった時(倒す)
	CheckPlayerBulletEnemy();
	// プレイヤーの弾が壊せるブロックに当たった時
	//CheckPlayerBulletBreakBlock();
}
