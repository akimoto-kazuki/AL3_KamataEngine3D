#pragma once
#include "LookOn.h"
#include "KamataEngine.h"
#include "MyMath.h"

#include <vector>

using namespace KamataEngine;
class Enemy;
class PlayerBullet 
{
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, const float theta);
	// 更新
	void Update();
	// 描画
	void Draw(const Camera& camera);
	// ワールドトランス処理
	void WorldTransformUpdate();
	void InputShot();

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	KamataEngine::Vector3 GetPlayerBulletPosition() const { return worldTransform_.translation_; }

	bool IsDead() const { return isDead_; }

private:
	
	// いつもの
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = { nullptr };
	uint32_t textureHandle_ = 0u;
	KamataEngine::Vector3 velocity_ = {};
	
	static const int32_t kLifeTime = 60 * 3;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

};
