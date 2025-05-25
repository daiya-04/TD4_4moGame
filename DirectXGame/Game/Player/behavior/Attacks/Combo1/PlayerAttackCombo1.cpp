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
}

void PlayerAttackCombo1::InitRigorAction()
{
}

void PlayerAttackCombo1::UpdatePreliminaryAction()
{
}

void PlayerAttackCombo1::UpdateAction()
{
}

void PlayerAttackCombo1::UpdateRigorAction()
{
}
