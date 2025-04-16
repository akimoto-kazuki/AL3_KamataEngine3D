#include "GameScene.h"
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

}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
}

void GameScene::Update() 
{
	player_->Update();
}

void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	player_->Draw(); 
	Model::PostDraw();
}
