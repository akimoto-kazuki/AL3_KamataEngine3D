#define NOMINMAX

#include "Enemy.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	
}

void Enemy::Update()
{
	std::array<Vector3, kNumCenter> positionsNew;
	// 移動後の４つ角の座標
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ , static_cast<Corner>(i));
	}
}

void Enemy::Draw()
{
	model_->Draw(worldTransform_, *camera_); 
}

KamataEngine::Vector3 Enemy::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{
	Vector3 offsetTable[kNumCenter] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
