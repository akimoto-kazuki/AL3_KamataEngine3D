#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() 
{
	textureHandle_ = TextureManager::Load("202.png"); 
	model_ = Model::Create();
	camera_.Initialize();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_,&camera_);

	// ブロック
	modelBlock_ = Model::CreateFromOBJ("cube");
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockWidth = 2.0f;
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) 
	{
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
	for (WorldTransform* worldTransformBlock: worldTransformBlocks_ ) 
	{
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

void GameScene::Update() 
{
	player_->Update();
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) 
	{
		worldTransformBlock->matWorld_ = 
			MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		worldTransformBlock->TransferMatrix();
	}
	
}

void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	player_->Draw();
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		modelBlock_->Draw(*worldTransformBlock, camera_);
	}
	Model::PostDraw();

	
}
