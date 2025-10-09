#include "Shoot.h"
#include "cassert"
#include "MyMath.h"
#include <numbers>
#include "MapChipField.h"
#include <algorithm>

using namespace KamataEngine;

void Shoot::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position)
{
	isShoot = false;
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;
}

void Shoot::Update() 
{ 
	
}

void Shoot::Draw() 
{
	if (isShoot) 
	{
		model_->Draw(worldTransform_, *camera_); 
	}
}
