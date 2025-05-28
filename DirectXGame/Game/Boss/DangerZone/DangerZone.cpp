#include "DangerZone.h"

DangerZone::DangerZone(const DangerZoneParameters& param)
{
	parameters_ = param;
}

void DangerZone::Update()
{
	//時間加算
	sec_ ++;

	//時間の割合を取得（0.0f~1.0f）
	float t = sec_ / parameters_.maxDeadSec;
	//イージング
	float wide = Lerp(t,0.0f, parameters_.maxRadius);

	//サイズ設定
	parameters_.world.scale_ = { wide,1,wide };

	//時間経過でフラグON
	if (sec_ >= parameters_.maxDeadSec)
	{
		isDead_ = true;
	}

	//行列更新
	parameters_.world.UpdateMatrix();
}
