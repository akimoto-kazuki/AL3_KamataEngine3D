#include "GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() 
{
	textureHandle_ = TextureManager::Load("rumia.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
}

GameScene::~GameScene() 
{ 
	delete model_; 
}

void GameScene::Update() 
{

}

void GameScene::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_, textureHandle_);

	Model::PostDraw();
}

