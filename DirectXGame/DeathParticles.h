#pragma once
#include "KamataEngine.h"
#include <numbers>
#include <array>

class DeathParticles
{
private:

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	// 存続時間
	static inline const float kDuration = 1.0f;
	// 移動の速さ
	static inline const float kSpeed = 0.05f;
	// 分割した一個分の個数
	static inline const float kAngleUnit = (std::numbers::pi_v<float> * 2) / kNumParticles;

	bool isFinished_ = false;

	float counter_ = 0.0f;

	KamataEngine::ObjectColor objectColor_;

	KamataEngine::Vector4 color_;

public:

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();
};
