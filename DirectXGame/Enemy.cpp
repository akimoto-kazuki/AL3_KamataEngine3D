#define NOMINMAX

#include "Enemy.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	camera_ = camera;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() 
{

	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 20.0f;

	// 回転アニメーション
	worldTransform_.rotation_.x = std::sin(walkTimer_);

	/*float param = std::sin(walkTimer_);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = std::*/

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	
}

void Enemy::Draw() 
{
	model_->Draw(worldTransform_, *camera_);
}
