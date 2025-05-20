#include "BossIdle.h"
#include"Boss/Boss.h"
#include<memory>

BossIdle::BossIdle()
{
	tree_.name_ = "Idle";
	tree_.SetValue("second", &sec_);
}

void BossIdle::Init()
{
}

void BossIdle::Update()
{
	//時間経過で次の状態に
	if (boss_->parameters_.currentSec >= sec_) {
		boss_->behaviorRequest_ = Boss::Attack1;
	}
}
