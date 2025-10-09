#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class CameraRay 
{
public:
	void Update(int mouseX, int mouseY, int scrrenWidth, int screenHeight, const XMMATRIX& view, const XMMATRIX& proj);

	XMVECTOR GetOrigin() const { return m_rayOrigin; }
	XMVECTOR GetDirection() const { return m_rayDir; }

private:
	XMVECTOR m_rayOrigin = XMVectorZero();
	XMVECTOR m_rayDir = XMVectorZero();
};
