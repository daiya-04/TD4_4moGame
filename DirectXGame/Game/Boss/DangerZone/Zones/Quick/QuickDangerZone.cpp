#include "QuickDangerZone.h"

QuickDangerZone::QuickDangerZone(const DangerZoneParameters& param)
{
	IDangerZone::Init(param);
	//うるせぇ！！いいから削除だ！！
	isDead_ = true;
	isDraw_ = false;
}

void QuickDangerZone::Update()
{
}
