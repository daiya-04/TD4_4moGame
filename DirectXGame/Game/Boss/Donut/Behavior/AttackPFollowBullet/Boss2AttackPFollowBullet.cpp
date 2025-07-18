#include "Boss2AttackPFollowBullet.h"

Boss2AttackPFollowBullet::Boss2AttackPFollowBullet(BossParameters* param)
{
	param_ = param;

	

	tree_.name_ = "FollowBullet";
	tree_.SetValue("waitCount", &waitTime_);
	tree_.SetValue("countCount", &executeCount_);
	tree_.SetText("ZoneMoveParamIsIn「DangerZoneManager」");

}

void Boss2AttackPFollowBullet::InitBehavior0()
{
	//アニメーション変更
	param_->setAnimeName_ = "Donut_Attack2";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = preActionRate_;
}

void Boss2AttackPFollowBullet::InitBehavior1()
{
	//アニメーション変更
	//param_->setAnimeName_ = "DonutAttack1";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = preActionRate_;
	//球を生成
	param_->SpawnDangerZone_ = true;
}

void Boss2AttackPFollowBullet::UpdateBehavior0()
{
	param_->isLookAtPlayer_ = true;
	//時間で次状態へ
	if (param_->currentSec >= waitTime_) {
		countRequest_ = 1;
	}
}

void Boss2AttackPFollowBullet::UpdateBehavior1()
{

	if (param_->currentSec >= executeCount_) {
		//通常状態に移行
		param_->behaviorRequest_ = 0;
	}
}
