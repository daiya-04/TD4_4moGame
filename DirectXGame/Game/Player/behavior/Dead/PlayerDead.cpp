#include "PlayerDead.h"
#include"Player.h"

PlayerDead::PlayerDead()
{
	tree_.name_ = "Dead";
	tree_.SetValue("maxCount", &maxCount_);
	tree_.SetValue("animRate", &playRate_);
}

void PlayerDead::Init()
{
	//描画フラグON
	player_->SetDraw(true);
	//アニメーションと再生倍率設定
	player_->SetAnimationName(animeName_);
	player_->SetAnimationLeverage(playRate_);
}

void PlayerDead::Update()
{
	//カウントを増加
	count_++;
	//時間で移動可能に
	if (count_ >= maxCount_) {
		player_->SetDead(true);
	}
}
