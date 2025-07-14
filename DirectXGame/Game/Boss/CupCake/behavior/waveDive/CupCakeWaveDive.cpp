#include "CupCakeWaveDive.h"

CupCakeWaveDive::CupCakeWaveDive(BossParameters* parameters)
{
	param_ = parameters;


	tree_.name_ = "waveDive";
	tree_.SetValue("jumpHeight", &jumpHeight_);
	tree_.SetValue("jumpCount", &jumpCount_);
	tree_.SetValue("waitCount", &waitCount_);
	tree_.SetValue("fallSpd", &fallSpd_);
	tree_.SetValue("stopCount", &stopCount_);


}

void CupCakeWaveDive::InitBehavior0()
{
}

void CupCakeWaveDive::InitBehavior1()
{
	//待機時間の間に中央に移動
	waitVelo_ = Vector3{ 0,jumpHeight_,0 } - param_->world->translation_;
	waitVelo_ = waitVelo_/waitCount_;


}

void CupCakeWaveDive::InitBehavior2()
{
}

void CupCakeWaveDive::InitBehavior3()
{
}

void CupCakeWaveDive::UpdateBehavior0()
{
	param_->velocity_ = { 0,jumpHeight_/jumpCount_,0 };
	if (param_->world->translation_.y >= jumpHeight_) {
		countRequest_ = 1;
	}
}

void CupCakeWaveDive::UpdateBehavior1()
{
	//待機時間の間中央に移動させる
	param_->velocity_ = waitVelo_;

	if (param_->currentSec >= waitCount_) {
		countRequest_ = 2;
	}
}

void CupCakeWaveDive::UpdateBehavior2()
{
	//落下処理
	param_->velocity_ = Vector3{ 0,-1.0f,0 }*fallSpd_;
	//0以下で終了
	if (param_->world->translation_.y <= 0) {
		countRequest_ = 3;
	}
}

void CupCakeWaveDive::UpdateBehavior3()
{
	if (param_->currentSec >= stopCount_) {
		param_->behaviorRequest_ = 0;
	}
}
