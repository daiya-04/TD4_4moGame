#include "SpinAttack.h"
#include"Player/Player.h"
#include"ColliderManager.h"

SpinAttack::SpinAttack()
{
	tree_.name_ = "SpinAttack";

	tree_.SetValue("upSpeed", &upSpeed_);
	tree_.SetValue("downSpeed", &downSpeed_);
	tree_.SetValue("stopDistance", &stopDistance_);

	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("playerSpin", *player_->world_, radius_);
	collider_->ColliderOff();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {if (collider->GetTag() == "boss")collider_->ColliderOff(); });

}



void SpinAttack::InitPreliminaryAction()
{
	//飛行フラグON
	player_->parameters_.isFlying = true;
}

void SpinAttack::InitAction()
{
	//攻撃コライダーを有効化
	collider_->ColliderOn();
}

void SpinAttack::InitRigorAction()
{
	//攻撃コライダーを無効化
	collider_->ColliderOff();
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
		//攻撃コライダーを無効化
		collider_->ColliderOff();
	}
	collider_->Update();
}

void SpinAttack::UpdateRigorAction()
{
	player_->parameters_.isFlying = false;
	player_->behaviorRequest_ = Player::Behavior::Move;
}
