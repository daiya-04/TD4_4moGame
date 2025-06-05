#include "PlayerAttackCombo1.h"
#include "Game/Player/Player.h"

PlayerAttackCombo1::PlayerAttackCombo1()
{
	tree_.name_ = "Com1";
}

void PlayerAttackCombo1::InitPreliminaryAction()
{
	player_->SetAnimationName("PlayerAttack1",false);
}

void PlayerAttackCombo1::InitAction()
{
	//攻撃コライダーを有効化
	player_->SetAttackColliderActive(true);
}

void PlayerAttackCombo1::InitRigorAction()
{
	//攻撃コライダーを無効化
	//player_->SetAttackColliderActive(false);
}

void PlayerAttackCombo1::UpdatePreliminaryAction()
{
	player_->SetBody2Input();
}

void PlayerAttackCombo1::UpdateAction()
{
}

void PlayerAttackCombo1::UpdateRigorAction()
{
}
