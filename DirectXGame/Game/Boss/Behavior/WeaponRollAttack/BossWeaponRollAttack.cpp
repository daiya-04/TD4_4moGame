#include "BossWeaponRollAttack.h"
#include"Boss/Boss.h"

BossWeaponRollAttack::BossWeaponRollAttack()
{
	tree_.name_ = "weaponRollAttack";
	tree_.SetValue("waitCount", &waitTime_);
	tree_.SetValue("activeCount", &attackTime_);
	tree_.SetValue("afterCount", &endTime_);

}

void BossWeaponRollAttack::InitBehavior0()
{
	//予備動作初期化
	boss_->SetAnimationName("GentlmanAttack2", false);
}

void BossWeaponRollAttack::InitBehavior1()
{
}

void BossWeaponRollAttack::InitBehavior2()
{
}

void BossWeaponRollAttack::UpdateBehavior0()
{
	if (boss_->parameters_.currentSec >= waitTime_) {
		countRequest_ = 1;
	}
}

void BossWeaponRollAttack::UpdateBehavior1()
{
	if (boss_->parameters_.currentSec >= attackTime_) {
		countRequest_ = 2;
	}
}

void BossWeaponRollAttack::UpdateBehavior2()
{
	if (boss_->parameters_.currentSec >= endTime_) {
		boss_->behaviorRequest_ = Boss::Behavior::Idle;
	}
}
