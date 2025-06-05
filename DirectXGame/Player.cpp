#define NOMINMAX

#include "Player.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position) { 
	
	assert(model); 
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

}

void Player::Update() {

	// 1移動入力
	InputMove();
	// 2移動量を加味して衝突判定
	// 衝突情報の初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	MapHitCheck(collisionMapInfo);

	MapHitMove(collisionMapInfo);
	//worldTransform_.translation_ += velocity_;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	CheckMapCeiling(collisionMapInfo);

	AnimateTurn();

	bool landing = false;

	if (velocity_.y < 0) 
	{
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	if (onGround_) 
	{
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} 
	else 
	{
		if (landing)
		{
			// めり込み
			worldTransform_.translation_.y = 1.0f;
			// 摩擦
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	
}

void Player::InputMove() 
{
	if (onGround_) 
	{
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT) || KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT)) {
			KamataEngine::Vector3 acceleration = {};
			if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRiget) {
					lrDirection_ = LRDirection::kRiget;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} 
			else if (KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT)) 
			{
				if (velocity_.x > 0.0f)
				{
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft)
				{
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} 
		else 
		{
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (KamataEngine::Input::GetInstance()->PushKey(DIK_UP)) 
		{
			velocity_ += KamataEngine::Vector3(0, kJumpAcceleration, 0);
			
		}
	} 
	else // 空中
	{
		velocity_ += KamataEngine::Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::AnimateTurn() 
{
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

// 天井に接触している場合の処理
void Player::CheckMapCeiling(const CollisionMapInfo& info)
{
	// 天井に当たった？
	if (info.ceiling) 
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{
	Vector3 offsetTable[kNumCenter] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::MapHitCheckUP(CollisionMapInfo& info) 
{
	
	// 上昇アリ？
	if (info.move.y <= 0) 
	{
		return;
	}
	std::array<Vector3, kNumCenter> positionsNew;
	// 移動後の４つ角の座標
	for (uint32_t i = 0; i < positionsNew.size(); ++i) 
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipByIndexSetPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipByIndexSetPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}
	if (hit) 
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = 
			mapChipField_->GetMapChipByIndexSetPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}


void Player::MapHitCheck(CollisionMapInfo& info) 
{
	MapHitCheckUP(info); 
}

// 判定結果を反映して移動させる
void Player::MapHitMove(const CollisionMapInfo& info)
{ 
	worldTransform_.translation_ += info.move;
}

void Player::Draw() 
{
	model_->Draw(worldTransform_, *camera_); 
}


