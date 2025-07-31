#include "SinpleDangerZone.h"

SinpleDangerZone::SinpleDangerZone(const DangerZoneParameters& param)
{
	IDangerZone::Init(param);
}

void SinpleDangerZone::Update()
{

	if (isDead_)return;

	sec_++;

	if (behavior_ == Behavior::Warning) {
		//時間の割合を取得（0.0f~1.0f）
		float t = sec_ / param_.maxWarningCount;
		//イージング
		float wide = Lerp(t, 0.0f, param_.maxRadius);

		//サイズ設定
		param_.world.scale_ = { wide,1,wide };

		//時間経過でフラグON
		if (sec_ >= param_.maxWarningCount)
		{
			behavior_ = Behavior::FinalWarning;
			sec_ = 0;
		}
	}
	else {



		//時間の割合を取得（0.0f~1.0f）
		if (sec_ >= (param_.maxFinalWarningCount / param_.maxFinalWarningCount) * blinkingCount_) {
			blinkingCount_++;
			//描画フラグを反転
			if (isDraw_) {
				isDraw_ = false;
			}
			else {
				isDraw_ = true;
			}
		}

		if (sec_ >= param_.maxFinalWarningCount) {
			//時間経過でフラグON
			isDead_ = true;
			//プレイヤー高度にセット
			if (pPos_) {
				param_.world.translation_.y = pPos_->y;
			}

		}

	}
}
