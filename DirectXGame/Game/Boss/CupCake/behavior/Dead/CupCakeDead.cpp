#include "CupCakeDead.h"

CupCakeDead::CupCakeDead(BossParameters* param)
{
	param_ = param;
	tree_.name_ = "Dead";
	tree_.SetValue("count", &count_);
	tree_.SetValue("animeLeverage", &animeLeverage_);
}

void CupCakeDead::Init()
{
	//アニメーション変更
	param_->setAnimeName_ = "Capcake";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = animeLeverage_;
}

void CupCakeDead::Update()
{
	if (param_->currentSec >= count_) {
		param_->isDead_ = true;
	}
}
