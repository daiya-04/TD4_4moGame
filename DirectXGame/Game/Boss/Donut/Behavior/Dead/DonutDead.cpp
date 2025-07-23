#include "DonutDead.h"

DonutDead::DonutDead(BossParameters* param)
{
	param_ = param;
	tree_.name_ = "Dead";
	tree_.SetValue("count", &count_);
	tree_.SetValue("animeLeverage", &animeLeverage_);
}

void DonutDead::Init()
{
	//アニメーション変更
	param_->setAnimeName_ = "Donut_Down";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = animeLeverage_;
}

void DonutDead::Update()
{
	if (param_->currentSec >= count_) {
		param_->isDead_ = true;
	}
}
