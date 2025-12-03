#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

#include <vector>

namespace KamataEngine
{
	class Sprite;
}

using namespace KamataEngine;

class LookOn
{
public:
	~LookOn();
	// 初期化
	void Initialize(KamataEngine::Camera* camera, KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 追尾
	void HomingReticle();
	// UI描画
	void DrawUI();
	// マウスの移動
	void MouseMove();

	// ワールドトランス処理
	void WorldTransformUpdate();

	KamataEngine::Vector3 GetWorldPosition();

	// マウス座標のget
	float GetMousePosX() { return mousePos_.x; }
	float GetMousePosY() { return mousePos_.y; }

private:

	KamataEngine::Sprite* sprite2DReticle_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.2f;

	static inline const float kAttenuation = 0.2f;
	static inline const float kLimitRunSpeed = 0.1f;

	KamataEngine::Sprite* lookOnDraw;

	KamataEngine::Vector3 position_;

	Vector2 mousePosXY_;
	float mousePosZ_;
	Vector3 mousePos_;

};

