#include "SpinAttack.h"
#include"Player/Player.h"

SpinAttack::SpinAttack()
{
	tree_.name_ = "SpinAttack";

	tree_.SetValue("upSpeed", &upSpeed_);
	tree_.SetValue("downSpeed", &downSpeed_);
	tree_.SetValue("stopDistance", &stopDistance_);
}



void SpinAttack::InitPreliminaryAction()
{
	//飛行フラグON
	player_->parameters_.isFlying = true;
}

void SpinAttack::InitAction()
{
	//攻撃コライダーを有効化
	player_->SetAttackColliderActive(true);
}

void SpinAttack::InitRigorAction()
{
	//攻撃コライダーを無効化
	player_->SetAttackColliderActive(false);
}

void SpinAttack::UpdatePreliminaryAction()
{
	//上昇する
	player_->parameters_.velocity = Vector3(0, upSpeed_, 0);
}

void SpinAttack::UpdateAction()
{
	//ボス方向に突撃
	Vector3 velo = player_->Get2BossDirection();
	//ボスとの距離を計算
	float distance = velo.Length();
	if (distance > stopDistance_) {
		//ボスとの距離が攻撃をやめる距離より大きい場合、ボス方向に移動
		velo.Normalize();
		player_->parameters_.velocity += velo * downSpeed_;
	}
	else {
		//ボスとの距離が攻撃をやめる距離以下の場合、終了
		player_->parameters_.isFlying = false;
		player_->behaviorRequest_ = Player::Behavior::Move; // 通常状態に戻るリクエスト
	}
}

void SpinAttack::UpdateRigorAction()
{
	player_->parameters_.isFlying = false;
	player_->behaviorRequest_ = Player::Behavior::Move;
}
