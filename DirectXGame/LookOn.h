#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

#include <vector>

namespace KamataEngine
{
	class Sprite;
}

class LookOn
{
public:

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();
	// (仮)上下左右を押したら標準を動かす のちにマウスで動かせるようにする
	void InputMove();

	// ワールドトランス処理
	void WorldTransformUpdate();

	KamataEngine::Vector3 GetWorldPosition();

private:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.2f;

	static inline const float kAttenuation = 0.2f;
	static inline const float kLimitRunSpeed = 0.1f;

	KamataEngine::Sprite* lookOnDraw;

	KamataEngine::Vector3 position_;

};

