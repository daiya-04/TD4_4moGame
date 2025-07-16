#include "PlayerMove.h"
#include"Player/Player.h"

#include "EffectManager.h"


PlayerMove::PlayerMove()
{
	tree_.name_ = "Move";
	tree_.SetMonitorValue("MoveVelo", &debugInputMove);
	tree_.SetValue("speed",&moveSpped_);

	player_->SetAnimationName("PlayerIdle");
}

void PlayerMove::Init()
{
	//アニメーションを変更
	player_->SetAnimationName("PlayerIdle");
	isMove_ = false;
}

void PlayerMove::Update()
{

	//移動入力取得
	Vector3 move = player_->SetBody2Input();

	debugInputMove = move;

	//移動する
	player_->parameters_.velocity += move * moveSpped_;

	//もし回避入力があった&クールタイムが終わっている場合
	if (player_->GetInput()->GetInput(PlayerInput::Type::Roll)&&player_->parameters_.currentRollCount==0) {
		//状態リクエスト送信
		player_->behaviorRequest_ = Player::Behavior::Roll;
		EffectManager::GetInstance()->End("PlayerMoveEffect");
	}
	//もし攻撃入力があったら
	else if (player_->GetInput()->GetInput(PlayerInput::Type::Attack)) {
		//攻撃リクエスト
		player_->behaviorRequest_ = Player::Behavior::Attack;
		EffectManager::GetInstance()->End("PlayerMoveEffect");
	}

	emitPos_ = player_->GetWorld().GetWorldPos() - Vector3(0.0f, 1.0f, 0.0f);

	//入力がない場合
	if (move.x == 0 && move.y == 0 && move.z == 0) {
		
		if (isMove_) {
			isMove_ = false;
			//待機モーション
			player_->SetAnimationName("PlayerIdle");
			EffectManager::GetInstance()->End("PlayerMoveEffect");
		}
	}
	else {
		if (!isMove_) {
			isMove_ = true;
			//入力があった場合は歩行アニメーション
			player_->SetAnimationName("PlayerWalk");

			emitPos_ = player_->GetWorld().GetWorldPos() - Vector3(0.0f, 1.0f, 0.0f);
			EffectManager::GetInstance()->Start("PlayerMoveEffect", &emitPos_);
		}
	}
}
