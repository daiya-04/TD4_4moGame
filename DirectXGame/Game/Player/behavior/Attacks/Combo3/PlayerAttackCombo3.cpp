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
}

void PlayerAttackCombo3::InitRigorAction()
{
}

void PlayerAttackCombo3::UpdatePreliminaryAction()
{
}

void PlayerAttackCombo3::UpdateAction()
{
}

void PlayerAttackCombo3::UpdateRigorAction()
{
}
