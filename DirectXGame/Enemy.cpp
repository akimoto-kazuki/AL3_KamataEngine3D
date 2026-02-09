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
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -1.0f;
	camera_ = camera;
	hitTimer_ = 0.0f;
}

void Enemy::Update() 
{
	if (isHit_)
	{
		hitTimer_ += 1.0f;
	}
	if (hitTimer_ >= 30.0f)
	{
		isDead_ = true;
	}
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

}

void Enemy::Draw() 
{
	if (!isHit_ || int(hitTimer_) % 2 == 1)
	{
		model_->Draw(worldTransform_, *camera_);
	}
}

KamataEngine::Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 0.5f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 0.5f) / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const PlayerBullet* playerBullet)
{
	(void)playerBullet; 
	isHit_ = true;
}
