#include "PlayerAttackCombo2.h"
#include"Game/Player/Player.h"

PlayerAttackCombo2::PlayerAttackCombo2()
{
	tree_.name_ = "Com2";
}

void PlayerAttackCombo2::InitPreliminaryAction()
{
	player_->SetAnimationName("PlayerAttack2",false);
}

void PlayerAttackCombo2::InitAction()
{
	//攻撃コライダーを有効化
	player_->SetAttackColliderActive(true);
}

void PlayerAttackCombo2::InitRigorAction()
{
	//攻撃コライダーを無効化
	player_->SetAttackColliderActive(false);
}

void PlayerAttackCombo2::UpdatePreliminaryAction()
{
	player_->SetBody2Input();
}

void PlayerAttackCombo2::UpdateAction()
{
}

void PlayerAttackCombo2::UpdateRigorAction()
{
}
