#include "PlayerRoll.h"
#include"Player/Player.h"


PlayerRoll::PlayerRoll()
{
	tree_.name_ = "Roll";
	tree_.SetMonitorValue("CurrentCount", &currentCount_);
	tree_.SetMonitorValue("stVelo", &currentVelo_);
	tree_.SetMonitorValue("nowVelo", &currentVelo_);


	tree_.SetValue("count", &maxCount_);
	tree_.SetValue("speed", &startSpeed_);
	tree_.SetValue("cooldownCount", &cooldownCount_);
}

void PlayerRoll::Init()
{
	//入力方向チェック
	currentVelo_ = player_->SetBody2Input();

	//もし入力がない場合
	if (currentVelo_ == Vector3{0, 0, 0}) {
		//前に進む
		currentVelo_ = {0,0,1};
		
		//プレイヤー方向に向ける
		currentVelo_=TransformNormal(currentVelo_, player_->GetWorld().matWorld_);
	}

	//時間リセット
	currentCount_ = 0;

	player_->SetAnimationName("PlayerAvoidance");
}

//0-1
float EaseOutCirc(float x) {
	return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
}

void PlayerRoll::Update()
{
	//押されていなければ終了
	if (!player_->GetInput()->GetInput(PlayerInput::Roll)) {
		//通常状態にリクエスト
		player_->behaviorRequest_ = Player::Behavior::Move;
		//回避クールタイムを再セット
		player_->parameters_.currentRollCount = cooldownCount_;

		//処理を終える
		return;
	}

	//時間カウント
	currentCount_++;
	//最大値を超えないよう処理
	if(currentCount_>maxCount_){
		currentCount_ = maxCount_;
	}


	//向きチェック

	//パラメータ取得
	PlayerParameters params = player_->parameters_;

	//地面の向きベクトル作成
	//Vector3 grandVelo = params.grandVelo.Normalize()*params.grandInfluence;

	//速度量を取得
	float t =( currentCount_ / maxCount_);

	

	//イージング関数を適用
	t = EaseOutCirc(t);
	//速度取得
	float spd  = Lerp(t, startSpeed_,0.0f);

	//移動する
	player_->parameters_.velocity += currentVelo_ * spd;


}
