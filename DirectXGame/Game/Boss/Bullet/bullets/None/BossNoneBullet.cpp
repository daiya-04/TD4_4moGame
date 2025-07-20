#include "BossNoneBullet.h"



BossNoneBullet::BossNoneBullet(const BossBulletData& data, DaiEngine::Camera* camera)
{
	IBossBullet::Init(data, camera);
	data_.world.translation_.y = 0;
}

void BossNoneBullet::Update()
{
	//行列更新のみ
	IBossBullet::UpdateObject();
}
