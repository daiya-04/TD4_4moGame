#include "BossFallBullet.h"
#include "EffectManager.h"

BossFallBullet::BossFallBullet(const BossBulletData& data, DaiEngine::Camera* camera)
{
	IBossBullet::Init(data, camera);
}

void BossFallBullet::Update()
{
	//加速処理
	data_.velocity += data_.acceraletion;
	//移動処理
	data_.world.translation_ += data_.velocity;

	//ワールドとコライダー更新
	IBossBullet::UpdateObject();
}

void BossFallBullet::AddOnCollisionBlock()
{
	//エフェクトを生成
	EffectManager::GetInstance()->Trigger("CandyFinishEffect", data_.world.GetWorldPos());
}
