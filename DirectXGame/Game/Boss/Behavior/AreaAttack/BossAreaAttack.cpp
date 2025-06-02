#include "BossAreaAttack.h"
#include"Boss/Boss.h"

BossAreaAttack::BossAreaAttack()
{
	tree_.name_ = "ItemFallAttack";
	tree_.SetValue("WaitCount", &waitTime_);
	tree_.SetValue("ActiveCount", &attackTime_);
	tree_.SetValue("Num", &numbers_);
	tree_.SetValue("PreActionRate_", &preActionRate_);
	tree_.SetValue("ActionRate_", &actionRate_);
}

void BossAreaAttack::InitBehavior0()
{
	//アニメーション変更
	boss_->SetAnimationName("GentlmanAttackPosture1",false);
	//再生速度変更
	boss_->SetAnimationLeverage(preActionRate_);

	boss_->SetCameraState(FollowCamera::State::None);
}

void BossAreaAttack::InitBehavior1()
{
	//実際の行動初期化
	currentNum_ = 0;
	//アニメーション変更
	boss_->SetAnimationName("GentlmanAttack1", false);
	//再生速度変更
	boss_->SetAnimationLeverage(actionRate_);
}

void BossAreaAttack::UpdateBehavior0()
{
	//時間外で次
	if (boss_->parameters_.currentSec > waitTime_) {
		countRequest_ = 1;
	}
}

void BossAreaAttack::UpdateBehavior1()
{	
	//経過時間で終了
	if (boss_->parameters_.currentSec >= attackTime_) {
		//通常状態に移行
		boss_->behaviorRequest_ = Boss::Behavior::Idle;
		//カメラ状態を変更
		boss_->SetCameraState(FollowCamera::State::Follow);
	}

	//タイム以上で発生
	if (boss_->parameters_.currentSec >= (attackTime_ / numbers_) * currentNum_) {
		//カウント増加
		currentNum_++;

		//生成
		boss_->SpawnDangerZone();
	}
}


