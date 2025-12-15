#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;


void GameScene::Initialize() {
	
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
	modelDeath_ = Model::CreateFromOBJ("deathParticle", true);
	// ブロック
	modelBlock_ = Model::CreateFromOBJ("block", true);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	TextureManager::Load("lookOn.png");

	// 自キャラの生成
	player_ = new Player();

	lookOn_ = new LookOn();

	deathParticles_ = new DeathParticles();

	unsigned int currentTime = unsigned int(time(nullptr));
	srand(currentTime);

	countMin = 30;
	isCountDown = true;

	for (int32_t i = 0; i < enemySpoon; ++i) 
	{
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + (5 * i), (rand() % 11 + 8));
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// カメラ
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	CameraController::Rect cameraArea = {12.0f, (100 - 12.0f), 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	Vector3 lookOnPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	//Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(18, 18);

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetLookOn(lookOn_);
	player_->SetMapChipField(mapChipField_);
	
	deathParticles_->Initialize(modelDeath_, &camera_, playerPosition);

	lookOn_->Initialize(&camera_, lookOnPosition);

	//デバッグ
	debugCamera_ = new DebugCamera(1280, 720);

	imGuiManager = ImGuiManager::GetInstance();
	
}

void GameScene::GenerateBlocks() 
{
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
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
	delete deathParticles_;
	delete fade_;
	delete lookOn_;
}

void GameScene::Update()
{

	ChangePhase();
	
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
		if (isCountDown)
		{
			countTimer++;
			if (countTimer >= 59)
			{
				countTimer = 0;
				countMin-=1;
			}
			if (countMin <= 0)
			{
				phase_ = Phase::kClearFadeOut;
			}
		}
		if (player_->IsDead() == true) 
		{
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeath_, &camera_, deathParticlesPosition);
		};
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_R))
		{
			finished_ = true;
		}
		
		break;

	case GameScene::Phase::kDeath:
		isCountDown = false;
		deathParticles_->Update();
		if (deathParticles_ && deathParticles_->IsFinished()) 
		{
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
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
		if (fade_->IsFinished()) {
			finished_ =  true;
		}
		break;
	case GameScene::Phase::kClearFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;

	}

	player_->Update();
	lookOn_->Update();
	skydome_->Update();
	for (Enemy* enemy : enemies_) 
	{
		enemy->Update();
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

void GameScene::ChangePhase() {

	switch (phase_) {

	case GameScene::Phase::kPlay:
		if (player_->isDead_)
		{
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_->Initialize(modelDeath_, &camera_, deathParticlesPosition);
		}
		break;

	case GameScene::Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
	
}

void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kClearFadeOut) 
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
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	lookOn_->DrawUI();

	Sprite::PostDraw();

	
	imGuiManager->Draw();
	

	fade_->Draw();
	
}

void GameScene::CheckAllCollisions()
{
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullets();

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	#pragma region 自キャラと敵キャラの当たり判定

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_)
	{
		aabb2 = enemy->GetAABB();
		
		// AABB同士の交差判定
		if (IsCollision(aabb1,aabb2))
		{
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);
		}
	}

	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定

	// 判定対象1と2の座標
	AABB aabb3, aabb4;
	for (Enemy* enemy : enemies_) 
	{
		aabb3 = enemy->GetAABB();
		for (PlayerBullet* playerBullet : playerBullets)
		{
			aabb4 = playerBullet->GetAABB();

			if (IsCollision(aabb3, aabb4))
			{
				enemy->OnCollision(playerBullet);

				playerBullet->OnCollision(enemy);
			}
		}
	}
	

	#pragma endregion

	#pragma region 自キャラとアイテムの当たり判定

	#pragma endregion

}
