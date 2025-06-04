#include "PlayerEntry.h"
#include"../../Player.h"

PlayerEntry::PlayerEntry()
{
	tree_.name_ = "playerEntry";
	tree_.SetValue("maxRate", &maxCount_);
	tree_.SetValue("animRate", &playRate_);
}

void PlayerEntry::Init()
{
	//描画フラグON
	player_->SetDraw(true);
	//アニメーションと再生倍率設定
	player_->SetAnimationName("PlayerEntry");
	player_->SetAnimationLeverage(playRate_);
}

void PlayerEntry::Update()
{
	//カウントを増加
	count_++;
	//時間で移動可能に
	if(count_ >= maxCount_) {
		player_->behaviorRequest_ = Player::Behavior::Move;
	}
}
