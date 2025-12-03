#include "LookOn.h"
#include "cassert"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

LookOn::~LookOn() 
{ delete sprite2DReticle_; }

void LookOn::Initialize(KamataEngine::Camera* camera, KamataEngine::Vector3& position) {
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
	sprite2DReticle_->SetSize({64.0f, 64.0f});
}

void LookOn::Update()
{
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

	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);
	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	// マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = camera_->matView * camera_->matProjection * matViewport;
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	// スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0);
	Vector3 posFar = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 1);
	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 2.0f;
	worldTransform_.translation_ = posNear + mouseDirection * kDistanceTestObject;
}

void LookOn::DrawUI()
{
	sprite2DReticle_->Draw(); 
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

