#include "CupCakeFallAttack.h"

#include "EffectManager.h"

CupCakeFallAttack::CupCakeFallAttack(BossParameters* parameters)
{
	param_ = parameters;

	tree_.name_ = "jumpAttack";
	tree_.SetValue("waitCount", &waitCount_);
	tree_.SetValue("attackCount", &attackCount_);
	tree_.SetValue("jumpHeight", &jumpHeight_);
	tree_.SetValue("jumpNum", &jumpNum_);
	tree_.SetValue("stopCount", &stopCount_);
}

void CupCakeFallAttack::InitBehavior0()
{
	//ステリセット
	currentJumpNum_ = 0;
	countRequest_ = 1;
}

void CupCakeFallAttack::InitBehavior1()
{
	//ジャンプ前
}

void CupCakeFallAttack::InitBehavior2()
{
	//ジャンプ処理
	currentJumpNum_++;

	basePos = param_->world->translation_;

	//ターゲット座標取得
	targetPos = param_->playerWorld_->translation_;
}

void CupCakeFallAttack::InitBehavior3()
{
	//ジャンプ後交直
}



void CupCakeFallAttack::UpdateBehavior1()
{
	param_->isLookAtPlayer_ = true;

	if (param_->currentSec >= waitCount_) {
		countRequest_ = 2;
	}
}

void CupCakeFallAttack::UpdateBehavior2()
{
	float t = param_->currentSec / attackCount_;

	Vector3 newpos = Lerp(t, basePos, targetPos);
	//放物線の高さを加える
	newpos.y += jumpHeight_ * (1.0f - std::abs(2.0f * t - 1.0f));

	//現在の座標との差分を求める
	newpos = newpos - param_->world->translation_;

	param_->velocity_ = newpos;

	if (param_->currentSec >= attackCount_) {
		if (currentJumpNum_ >= jumpNum_) {
			countRequest_ = 3;
			//EffectManager::GetInstance()->Trigger("CapCakeStampEffect", param_->world->translation_ - Vector3(0.0f, 1.0f, 0.0f));
		}
		else {
			//前の状態に戻る
			countRequest_ = 1;
		}
		EffectManager::GetInstance()->Trigger("CapCakeStampEffect", param_->world->translation_ - Vector3(0.0f, 1.0f, 0.0f));
	}
}

void CupCakeFallAttack::UpdateBehavior3()
{
	if (param_->currentSec >= stopCount_) {
		param_->behaviorRequest_=0;
	}
}
