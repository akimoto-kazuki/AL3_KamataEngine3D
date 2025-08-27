#pragma once

#include "KamataEngine.h"
#include "MyMath.h"

#include <vector>


class Goal 
{
private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();

	/*KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();*/
};
