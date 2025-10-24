#include "LookOn.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void LookOn::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

	position_ = {position.x, position.y};
	Vector4 collar = {1, 1, 1, 1};
	int GH = TextureManager::Load("lookOn.png");
	lookOnDraw = Sprite::Create(GH, {position_.x,position_.y}, collar, {0.0f, 0.0f}, false, false);

}

void LookOn::Update()
{
	InputMove(); 
	WorldTransformUpdate();
}

void LookOn::Draw() 
{ 
	model_->Draw(worldTransform_, *camera_); 
	//lookOnDraw->Draw();
}

void LookOn::InputMove()
{
	if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT) || KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT))
	{
		KamataEngine::Vector3 acceleration = {};
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;

		} else if (KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_ += acceleration;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} 
	else
	{
		velocity_.x *= (1.0f - kAttenuation);
	}
	if (KamataEngine::Input::GetInstance()->PushKey(DIK_UP) || KamataEngine::Input::GetInstance()->PushKey(DIK_DOWN)) {
		KamataEngine::Vector3 acceleration = {};
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_UP)) {
			if (velocity_.y < 0.0f)
			{
				velocity_.y *= (1.0f - kAttenuation);
			}
			acceleration.y += kAcceleration;

		} else if (KamataEngine::Input::GetInstance()->PushKey(DIK_DOWN)) {
			if (velocity_.y > 0.0f) {
				velocity_.y *= (1.0f - kAttenuation);
			}
			acceleration.y -= kAcceleration;
		}
		velocity_ += acceleration;
		velocity_.y = std::clamp(velocity_.y, -kLimitRunSpeed, kLimitRunSpeed);

	} else {
		velocity_.y *= (1.0f - kAttenuation);
	}
}

void LookOn::WorldTransformUpdate()
{
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	/*position_.x += velocity_.x;
	position_.y += velocity_.y*/;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

KamataEngine::Vector3 LookOn::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
