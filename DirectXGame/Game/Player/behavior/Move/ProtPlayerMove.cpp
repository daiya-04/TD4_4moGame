#include "ProtPlayerMove.h"
#include"Player/Player.h"


PlayerMove::PlayerMove()
{
	tree_.name_ = "Move";
	tree_.SetValue("speed",&moveSpped_);

}

void PlayerMove::Init()
{
}

void PlayerMove::Update()
{

	//移動入力取得
	Vector3 move = player_->SetBody2Input();

	//移動する
	player_->GetWorld().translation_ += move * moveSpped_;

	//もし回避入力があった&クールタイムが終わっている場合
	if (player_->GetInput()->GetInput(PlayerInput::Type::Roll)&&player_->parameters_.currentRollCount==0) {
		//状態リクエスト送信
		player_->behaviorRequest_ = Player::Behavior::Roll;
	}
	//もし攻撃入力があったら
	else if (player_->GetInput()->GetInput(PlayerInput::Type::Attack)) {
		//攻撃リクエスト
		player_->behaviorRequest_ = Player::Behavior::Attack;
	}
}
