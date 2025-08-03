#include "GingerbreadManDead.h"

#include "EffectManager.h"

GingerbreadManDead::GingerbreadManDead(BossParameters* param)
{
	param_ = param;
	tree_.name_ = "Dead";
	tree_.SetValue("count", &count_);
	tree_.SetValue("animeLeverage", &animeLeverage_);
}

void GingerbreadManDead::Init()
{
	//アニメーション変更
	param_->setAnimeName_ = "GentlmanDead";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = animeLeverage_;

	EffectManager::GetInstance()->End("StickAttackEffect");

}

void GingerbreadManDead::Update()
{
	if (param_->currentSec >= count_) {
		param_->isDead_ = true;
	}
}
