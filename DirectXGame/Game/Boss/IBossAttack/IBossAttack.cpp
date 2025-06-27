#include "IBossAttack.h"
#include"Boss/Boss1/Boss.h"

IBossAttack::IBossAttack()
{
	tree_.SetMonitorValue("nowBehavior", &behaviorCount_);

}

void IBossAttack::Init()
{
	behaviorCount_ = 0;
	countRequest_ = 0;
}


void IBossAttack::Update()
{
	if (countRequest_) {
		behaviorCount_ = countRequest_.value();
		countRequest_ = std::nullopt;
		param_->currentSec = 0;
		//初期化
		(this->*behaviorInit[behaviorCount_])();
	}

	//更新
	(this->*behaviorUpdate[behaviorCount_])();

}


//初期化関数をまとめる
void (IBossAttack::* IBossAttack::behaviorInit[])() {
	&IBossAttack::InitBehavior0,
	& IBossAttack::InitBehavior1,
	& IBossAttack::InitBehavior2,
	& IBossAttack::InitBehavior3,
	& IBossAttack::InitBehavior4,

};

//更新処理関数をまとめる
void(IBossAttack::* IBossAttack::behaviorUpdate[])() {
	&IBossAttack::UpdateBehavior0,
	& IBossAttack::UpdateBehavior1,
	& IBossAttack::UpdateBehavior2,
	& IBossAttack::UpdateBehavior3,
	& IBossAttack::UpdateBehavior4,

};