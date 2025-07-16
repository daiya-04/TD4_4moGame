#include "BossBullet.h"
#include"ColliderManager.h"

#include "EffectManager.h"

BossBullet::BossBullet(const BossBulletData& data)
{
	type_ = data.type;
	//弾判定初期化
	world_ = data.world;
	velocity_ = data.velocity;
	acceleration_ = data.acceraletion;
	warningWorld_ = data.warningWorld;

	//サイズ設定
	world_.scale_={ data.radius,data.radius ,data.radius };

	//コライダー初期化
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("敵の弾",world_,data.colliderRadius);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider*) { OnCollision(); });
	//コライダーを有効に
	collider_->ColliderOn();
}

BossBullet::~BossBullet()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void BossBullet::Update()
{
	//加速処理
	velocity_ += acceleration_;
	//移動処理
	world_.translation_ += velocity_;

	//ブロックにあたらなかった場合の削除処理(仮)
	if (world_.translation_.y < -10) {
		isDead_ = true;
	}

	//ワールド更新
	world_.UpdateMatrix();
	//コライダー更新
	collider_->Update();
}

void BossBullet::OnCollision()
{
	//死亡フラグON
	isDead_ = true;
	//コライダーを無効にする
	collider_->ColliderOff();

	EffectManager::GetInstance()->Start("CandyFinishEffect", world_.GetWorldPos());
}

