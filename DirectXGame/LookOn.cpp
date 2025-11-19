#include "LookOn.h"
#include "cassert"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

LookOn::~LookOn() 
{ delete sprite2DReticle_; }

void LookOn::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

	position_ = {position.x, position.y};
	mousePosXY_ = {position_.x,position_.y};
	mousePosZ_ = 0;

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("lookOn.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f,310.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

void LookOn::Update()
{
	InputMove(); 
	//MouseMove();

	WorldTransformUpdate();
	HomingReticle();
}

void LookOn::HomingReticle()
{
	Vector3 positionReticle = GetWorldPosition(); 
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成
	// AL3 03_14 p35
	Matrix4x4 matViewProjectionViewport = camera_->matView * camera_->matProjection * matViewport;
	// ワールド→Screen座標変換
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void LookOn::Draw() 
{ 
	model_->Draw(worldTransform_, *camera_); 
}

void LookOn::DrawUI()
{
	sprite2DReticle_->Draw(); 
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
#include <iostream>
void LookOn::MouseMove()
{
	mousePosXY_ = Input::GetInstance()->GetMousePosition(); 
	std::cout << "mouse x" << mousePosXY_.x << ", " << mousePosXY_.y << std::endl;
	mousePos_ = {mousePosXY_.x, mousePosXY_.y, mousePosZ_};
	velocity_ += mousePos_;
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

