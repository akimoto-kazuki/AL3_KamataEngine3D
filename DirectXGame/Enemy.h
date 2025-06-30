#pragma once
#include "KamataEngine.h"
#include <vector>
class Enemy 
{

	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCenter // 要素数
	};

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformPlayer_;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

};
