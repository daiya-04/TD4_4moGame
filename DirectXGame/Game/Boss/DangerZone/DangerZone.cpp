#include "DangerZone.h"

DangerZone::DangerZone(const DangerZoneParameters& param)
{
	parameters_ = param;
}

void DangerZone::Reset() {
	sec_ = 0;
	isDead_ = false;
	behavior_ = Behavior::Warning;
	isDraw_ = true;
	blinkingCount_ = 0;
}

void DangerZone::Update()
{
	//時間加算
	sec_ ++;

	//警告状態の処理
	if(behavior_== Behavior::Warning) {
		//通常時処理
		if (parameters_.type == BulletType::Fall||parameters_.type ==BulletType::Parabola) {
			NormalUpdate();
		}//追従時処理
		else if (parameters_.type == BulletType::None) {
			FollowUpdate();
		}
	}else if (behavior_ == Behavior::FinalWarning) {
		//時間の割合を取得（0.0f~1.0f）
		if (sec_ >= (parameters_.maxFinalWarningCount / parameters_.maxFinalWarningCount) * blinkingCount_) {
			blinkingCount_++;
			//描画フラグを反転
			if (isDraw_) {
				isDraw_ = false;
			}
			else {
				isDraw_ = true;
			}
		}

		if(sec_>= parameters_.maxFinalWarningCount) {
			//時間経過でフラグON
			isDead_ = true;
			isDraw_ = false; // 描画フラグをリセット
			//プレイヤー高度にセット
			parameters_.world.translation_.y=pPos_->y;
			
		}
	}




	//行列更新
	parameters_.world.UpdateMatrix();
}

void DangerZone::NormalUpdate()
{
	//時間の割合を取得（0.0f~1.0f）
	float t = sec_ / parameters_.maxWarningCount;
	//イージング
	float wide = Lerp(t, 0.0f, parameters_.maxRadius);

	//サイズ設定
	parameters_.world.scale_ = { wide,1,wide };

	//時間経過でフラグON
	if (sec_ >= parameters_.maxWarningCount)
	{
		behavior_ = Behavior::FinalWarning;
		sec_ = 0;
	}
}

void DangerZone::FollowUpdate()
{
	//時間の割合を取得（0.0f~1.0f）
	float t = sec_ / parameters_.maxWarningCount;
	//イージング
	float wide = Lerp(t, 0.0f, parameters_.maxRadius);

	//サイズ設定
	parameters_.world.scale_ = { wide,1,wide };

	//追従処理
	//プレイヤー方向を取得
	Vector3 playerPos = *pPos_-parameters_.world.translation_;
	//正規化
	playerPos = playerPos.Normalize();
	//速度をかけて加算
	parameters_.world.translation_ += playerPos * parameters_.followSpd;

	//時間経過でフラグON
	if (sec_ >= parameters_.maxWarningCount)
	{
		behavior_ = Behavior::FinalWarning;
		sec_ = 0;
	}
}
