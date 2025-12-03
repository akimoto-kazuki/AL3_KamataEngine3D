#include "PlayerBullet.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;
// 初期化
void PlayerBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	velocity_ = velocity;
}
// 更新
void PlayerBullet::Update() 
{
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
	worldTransform_.translation_ += velocity_;
	WorldTransformUpdate();
	
}
// 描画
void PlayerBullet::Draw(const Camera& camera) 
{
	model_->Draw(worldTransform_, camera, textureHandle_);
}

KamataEngine::Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB PlayerBullet::GetAABB() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 0.5f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 0.5f) / 2.0f};

	return aabb;
}

void PlayerBullet::WorldTransformUpdate()
{
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PlayerBullet::InputShot() 
{
	
}

void PlayerBullet::OnCollision(const Enemy* enemy) 
{
	(void)enemy;
	isDead_ = true;
}
