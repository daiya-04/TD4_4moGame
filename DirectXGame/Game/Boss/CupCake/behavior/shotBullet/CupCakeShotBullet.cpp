#include "CupCakeShotBullet.h"

CupCakeShotBullet::CupCakeShotBullet(BossParameters* parameters)
{
	param_ = parameters;

	tree_.name_ = "ShotBullet";
	tree_.SetValue("waitCount", &waitCount_);
	tree_.SetValue("attackCount", &attackCount_);
}

void CupCakeShotBullet::InitBehavior0()
{
	////アニメーション変更
	//param_->setAnimeName_ = "GentlmanAttackPosture1";
	//param_->isLoopAnime_ = false;
	////再生速度変更
	//param_->animationLeverage_ = preActionRate_;

	//カメラの状態リクエスト
	//param_->cameraBehaviorRequest_ = FollowCamera::State::None;
	
	//生成
	param_->bulletTypeRequest_ = BulletType::Parabola;
	//警告円生成
	param_->SpawnDangerZone_ = true;
}

void CupCakeShotBullet::InitBehavior1()
{
	////アニメーション変更
	//param_->setAnimeName_ = "GentlmanAttack1";
	//param_->isLoopAnime_ = false;
	////再生速度変更
	//param_->animationLeverage_ = actionRate_;
}

void CupCakeShotBullet::UpdateBehavior0()
{
	//時間外で次
	if (param_->currentSec > waitCount_) {
		countRequest_ = 1;
	}
}

void CupCakeShotBullet::UpdateBehavior1()
{
	//経過時間で終了
	if (param_->currentSec >= attackCount_ && param_->currentBulletNum_ <= 0) {
		//通常状態に移行
		param_->behaviorRequest_ = 0;
		//カメラ状態を変更
		//param_->cameraBehaviorRequest_ = FollowCamera::State::Follow;
	}
}
