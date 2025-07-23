#include "PlayerMove.h"
#include"Player/Player.h"

#include "EffectManager.h"


PlayerMove::PlayerMove()
{
	tree_.name_ = "Move";
	tree_.SetMonitorValue("MoveVelo", &debugInputMove);
	tree_.SetValue("speed",&moveSpped_);
	tree_.SetValue("jumpStSpd", &stSpd_);
	tree_.SetValue("degreeSpd", &gravity_);
	tree_.SetValue("degreeGravityOnTop", &degreeGravityOnTop_);

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
	JumpUpdate(move);
	debugInputMove = move;
	
	//移動する
	player_->parameters_.velocity += move * moveSpped_;


	if (!isJump_) {
		BehaviorChange();
	}

	MoveVeloUpdate(move);

}

void PlayerMove::JumpUpdate(Vector3&move)
{
	//ジャンプ処理開始
	if (!isJump_&&player_->GetInput()->GetInput(PlayerInput::Type::Jump)) {
		isJump_ = true;
		nowSpd_ = stSpd_;
		player_->parameters_.isFlying = true;
	}

	if (isJump_) {		
		move.y = nowSpd_;
		if(nowSpd_<=1.0f&&nowSpd_>=-1.0f){
			nowSpd_ -= gravity_/degreeGravityOnTop_;
		}
		else {
			nowSpd_ -= gravity_;
		}

		//仮終了処理
		if (player_->world_->translation_.y< player_->parameters_.blockY) {
			player_->parameters_.isFlying = false;
			isJump_ = false;
		}
	}

}

void PlayerMove::BehaviorChange()
{
	//もし回避入力があった&クールタイムが終わっている場合
	if (player_->GetInput()->GetInput(PlayerInput::Type::Roll) && player_->parameters_.currentRollCount == 0) {
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
}

void PlayerMove::MoveVeloUpdate(const Vector3 &move)
{
	if (isMove_) {
		Vector3 emitPos = player_->GetWorld().GetWorldPos() - Vector3(0.0f, 1.0f, 0.0f);
		EffectManager::GetInstance()->Start("PlayerMoveEffect", &emitPos);
	}
	emitPos_ = player_->GetWorld().GetWorldPos() - Vector3(0.0f, 1.0f, 0.0f);

	//ジャンプ中ならスキップ
	if (isJump_)return;

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
