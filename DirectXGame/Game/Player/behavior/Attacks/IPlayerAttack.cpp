#include "IPlayerAttack.h"

#include"Player/Player.h"

IPlayerAttack::IPlayerAttack()
{
	//入力クラスインスタンス生成
	input_ = std::make_unique<PlayerInput>();

	//デバッグ用にパラメータ設定
	std::vector<std::string> behaviorName_ = {
		"PreAttack",
		"Attack",
		"EndAttack"
	};
	tree_.SetMonitorValue("CurrentCount", &currentCount_);
	tree_.SetMonitorCombo("Behavior",&behavior_,behaviorName_);
	tree_.SetValue("AnimationFactor", &playbackFactor_);
	//各最大時間パラメータを設定
	for (int i = 0; i < Count; i++) {
		tree_.SetValue(behaviorName_[i], &maxCounts_[i]);

	}
}

void IPlayerAttack::Init()
{
	//共通初期化処理
	//初期状態に
	behavior_ = 0;
	//リクエストを初期に設定
	behaviorRequest_ = 0;
	//次攻撃入力フラグ初期化
	isAttack_ = false;
	//終了フラグ初期化
	isEnd_ = false;
}

void IPlayerAttack::Update()
{
	//上大陸ストで処理
	if (behaviorRequest_) {
		//状態を更新
		behavior_ = behaviorRequest_.value();
		//リクエストを初期化
		behaviorRequest_ = std::nullopt;
		//現在のカウントを初期化
		currentCount_ = 0;
		//現在のカウント最大値を状態の最大値に設定
		nowCount_ = maxCounts_[behavior_];
		//初期化
		(this->*behaviorInit[behavior_])();		
		//再生倍率を設定
		player_->SetAnimationLeverage(playbackFactor_);		
	}

	//更新
	(this->*behaviorUpdate[behavior_])();

	//時間更新
	currentCount_ ++;

	//時間経過で次の状態に移行
	if (currentCount_ >= nowCount_) {
		//次の状態に移行
		if (behavior_ < Count - 1) {
			//次の状態に移行
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