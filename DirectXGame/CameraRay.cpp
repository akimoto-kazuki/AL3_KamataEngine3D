#include "CameraRay.h"

void CameraRay::Update(int mouseX, int mouseY, int scrrenWidth, int screenHeight, const XMMATRIX& view, const XMMATRIX& proj) 
{
	float px = ((2.0f * mouseX) / scrrenWidth - 1.0f);
	float py = ((2.0f * mouseY) / screenHeight - 1.0f) * -1.0f;

	XMMATRIX invViewProj = XMMatrixInverse(nullptr, view * proj);

	XMVECTOR nearPoint = XMVector3TransformCoord(XMVectorSet(px, py, 0.0f, 1.0f), invViewProj);
	XMVECTOR farPoint = XMVector3TransformCoord(XMVectorSet(px, py, 1.0f, 1.0f), invViewProj);

	m_rayOrigin = nearPoint;
	m_rayDir = XMVector3Normalize(farPoint - nearPoint);

}
