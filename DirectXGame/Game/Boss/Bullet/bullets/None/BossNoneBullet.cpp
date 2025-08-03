#include "BossNoneBullet.h"
#include "EffectManager.h"
#include "AudioManager.h"

BossNoneBullet::BossNoneBullet(const BossBulletData& data, DaiEngine::Camera* camera)
{
	IBossBullet::Init(data, camera);
	data_.world.translation_.y = data.warningWorld.translation_.y;

	se_ = DaiEngine::AudioManager::Load("SE/dounutUpper.mp3");

}

void BossNoneBullet::Update()
{
	//行列更新のみ
	IBossBullet::UpdateObject();
}


void BossNoneBullet::AddOnCollisionBlock() {
	if (data_.type == BulletType::None) {
		//えふぇこ発生
		EffectManager::GetInstance()->Trigger("DonutsStageApperEffect", data_.world.GetWorldPos());
		se_->Play();
	}
}