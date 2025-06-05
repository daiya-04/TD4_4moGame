#include "PlayerAttackCombo3.h"
#include"Game/Player/Player.h"

PlayerAttackCombo3::PlayerAttackCombo3()
{
	tree_.name_ = "Com3";
}

void PlayerAttackCombo3::InitPreliminaryAction()
{
	player_->SetAnimationName("PlayerAttack1", false);
}

void PlayerAttackCombo3::InitAction()
{
	//攻撃コライダーを有効化
	player_->SetAttackColliderActive(true);
}

void PlayerAttackCombo3::InitRigorAction()
{
	//攻撃コライダーを無効化
	player_->SetAttackColliderActive(false);
}

void PlayerAttackCombo3::UpdatePreliminaryAction()
{
	player_->SetBody2Input();
}

void PlayerAttackCombo3::UpdateAction()
{
}

void PlayerAttackCombo3::UpdateRigorAction()
{
}
