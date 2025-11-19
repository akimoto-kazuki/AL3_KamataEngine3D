#pragma once
#include "KamataEngine.h"

KamataEngine::Matrix4x4
MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);
KamataEngine::Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
KamataEngine::Vector3 Transform(KamataEngine::Vector3& vector, KamataEngine::Matrix4x4& matrix);
float EaseInOut(float x1, float x2, float t);
struct AABB
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

class MyMath 
{

	
};
