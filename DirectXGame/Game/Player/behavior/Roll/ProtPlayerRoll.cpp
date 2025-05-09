#include "ProtPlayerRoll.h"
#include"Player/Player.h"


PlayerRoll::PlayerRoll()
{
	tree_.name_ = "Roll";
	tree_.SetMonitorValue("CurrentCount", &currentCount_);
	tree_.SetMonitorValue("RollVelo", &velo_);

	tree_.SetValue("count", &count_);
	tree_.SetValue("speed", &rollSpeed_);
	tree_.SetValue("cooldownCount", &cooldowmCount_);
}

void PlayerRoll::Init()
{
	//入力方向チェック
	velo_ = player_->SetBody2Input();

	//もし入力がない場合
	if (velo_ == Vector3{0, 0, 0}) {
		//前に進む
		velo_ = {0,0,1};
	}

	//時間リセット
	currentCount_ = 0;
}

void PlayerRoll::Update()
{

	//時間カウント
	currentCount_++;

	//時間外なら終了
	if (currentCount_ >= count_) {
		//通常状態にリクエスト
		player_->behaviorRequest_ = Player::Behavior::Move;
		//回避クールタイムを再セット
		player_->parameters_.currentRollCount = cooldowmCount_;
		
		//処理を終える
		return;
	}

	//時間内なので処理

	//移動する
	player_->GetWorld().translation_ += velo_ * rollSpeed_;
}
