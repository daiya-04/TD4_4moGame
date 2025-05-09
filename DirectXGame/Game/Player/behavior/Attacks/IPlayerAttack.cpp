#include "IPlayerAttack.h"

#include"Player/Player.h"

IPlayerAttack::IPlayerAttack()
{

	input_ = std::make_unique<PlayerInput>();

	std::vector<std::string> behaviorName_ = {
		"予備動作",
		"攻撃",
		"硬直"
	};

	tree_.SetMonitorValue("経過時間", &currentCount_);
	tree_.SetMonitorCombo("状態",&behavior_,behaviorName_);
	//各最大時間パラメータを設定
	for (int i = 0; i < Count; i++) {
		tree_.SetValue(behaviorName_[i], &maxCounts_[i]);
	}
}

void IPlayerAttack::Init()
{
	behavior_ = 0;
	behaviorRequest_ = 0;
	isAttack_ = false;
	isEnd_ = false;
}

void IPlayerAttack::Update()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;
		currentCount_ = 0;
		nowCount_ = maxCounts_[behavior_];
		//初期化
		(this->*behaviorInit[behavior_])();		
	}

	//更新
	(this->*behaviorUpdate[behavior_])();

	//時間更新
	currentCount_ ++;

	//時間経過で次の状態に移行
	if (currentCount_ >= nowCount_) {
		//次の状態に移行
		if (behavior_ < Count - 1) {
			behaviorRequest_ = behavior_ + 1;
		}
		else {
			isEnd_ = true;
		}
	}

	//もし次の攻撃入力があったら
	if (input_->GetInput(PlayerInput::Type::Attack)) {
		//次の攻撃フラグを立てる
		isAttack_ = true;
	}

}

//初期化関数をまとめる
void (IPlayerAttack::* IPlayerAttack::behaviorInit[])() {
	& IPlayerAttack::InitPreliminaryAction,
	& IPlayerAttack::InitAction,
	& IPlayerAttack::InitRigorAction
};

//更新処理関数をまとめる
void(IPlayerAttack::* IPlayerAttack::behaviorUpdate[])() {
	& IPlayerAttack::UpdatePreliminaryAction,
	& IPlayerAttack::UpdateAction,
	& IPlayerAttack::UpdateRigorAction,
};