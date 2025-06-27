#include "BossAreaAttack.h"
#include"Boss/Boss1/Boss.h"

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
	param_->setAnimeName_ = "GentlmanAttackPosture1";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ =preActionRate_;

	//カメラの状態リクエスト
	param_->cameraBehaviorRequest_ =FollowCamera::State::None;
}

void BossAreaAttack::InitBehavior1()
{
	//実際の行動初期化
	currentNum_ = 0;
	//アニメーション変更
	param_->setAnimeName_ = "GentlmanAttack1";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = actionRate_;
}

void BossAreaAttack::UpdateBehavior0()
{
	//時間外で次
	if (param_->currentSec > waitTime_) {
		countRequest_ = 1;
	}
}

void BossAreaAttack::UpdateBehavior1()
{	
	//経過時間で終了
	if (param_->currentSec >= attackTime_) {
		//通常状態に移行
		param_->behaviorRequest_ = Boss::Behavior::Idle;
		//カメラ状態を変更
		param_->cameraBehaviorRequest_ = FollowCamera::State::Follow;
	}

	//タイム以上で発生
	if (param_->currentSec >= (attackTime_ / numbers_) * currentNum_) {
		//カウント増加
		currentNum_++;

		//生成
		param_->SpawnDangerZone_ = true;
	}
}


