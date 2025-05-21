#include "BossBullet.h"

BossBullet::BossBullet(const BossBulletData& data)
{
	//弾判定初期化
	world_ = data.world;
	velocity_ = data.velocity;
	acceleration_ = data.acceraletion;


	world_.scale_={ data.radius,data.radius ,data.radius };

	//コライダー初期化
	/*colldier_ = std::make_unique<SphereCollider>();
	colldier_->Initialize("敵弾",world_);
	colldier_->SetRadius(data.radius);*/
}

void BossBullet::Update()
{
	//加速処理
	velocity_ += acceleration_;
	//移動処理
	world_.translation_ += velocity_;

	//コライダー更新
	//colldier_->Update();

	//仮の削除処理
	if (world_.translation_.y < -10) {
		isDead_ = true;
	}
}

void BossBullet::OnCollision()
{
	isDead_ = true;
}

