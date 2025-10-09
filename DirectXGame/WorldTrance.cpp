#include "WorldTrance.h"
#include "MyMath.h"
void WorldTrance::worldTranceForm(KamataEngine::WorldTransform& worldTransform) 
{
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	worldTransform.TransferMatrix();
}
