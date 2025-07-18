#include "BossBullet.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

BossBullet::BossBullet(const BossBulletData& data, DaiEngine::Camera* camera)
{
	camera_ = camera;

	type_ = data.type;
	//弾判定初期化
	world_ = data.world;
	velocity_ = data.velocity;
	acceleration_ = data.acceraletion;
	warningWorld_ = data.warningWorld;
	bossWorld_ = data.bossWorld;
	arriveCount_ = data.arriveCount;
	parabolaHeight_ = data.parabolaHeight;

	//サイズ設定
	world_.scale_={ data.radius,data.radius ,data.radius };

	//コライダー初期化
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("敵の弾",world_,data.colliderRadius);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {if (collider->GetTag() == "player")collider_->ColliderOff(); isDead_ = true; });
	//コライダーを有効に
	collider_->ColliderOn();

	//放射弾は最初判定なし
	if(type_ == BulletType::Parabola) {
		//放物線用の初期化
		currentCount_ = 0;
		isActive_ = false;
	}

}

BossBullet::~BossBullet()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void BossBullet::Update()
{
	if (type_ != BulletType::Parabola) {
		//加速処理
		velocity_ += acceleration_;
		//移動処理
		world_.translation_ += velocity_;
	}
	else {
		//放物線を描く弾の処理
		currentCount_++;

		if (!isActive_ && currentCount_ >= ((float)arriveCount_ / 2.0f))isActive_ = true;

		//進行度T取得
		float t = (float)currentCount_ / (float)arriveCount_;
		//X,Zの移動処理
		float x = Lerp(t,bossWorld_.translation_.x, warningWorld_.translation_.x);
		float z = Lerp(t, bossWorld_.translation_.z, warningWorld_.translation_.z);

		//Yの移動処理(真ん中で最高高度
		float y = Lerp(t, bossWorld_.translation_.y, warningWorld_.translation_.y);
		//放物線の高さを加える
		y += parabolaHeight_ * (1.0f - std::abs(2.0f * t - 1.0f));
		//ワールドの更新
		world_.translation_ = { x, y, z };

		if(currentCount_>=arriveCount_) {
			//到着したら削除
			//isDead_ = true;
			//コライダーを無効にする
			//collider_->ColliderOff();
			//return;
		}
	}

	//ブロックにあたらなかった場合の削除処理(仮)
	if (world_.translation_.y < -10) {
		isDead_ = true;
		return;
	}

	//ワールド更新
	world_.UpdateMatrix();
	//コライダー更新
	collider_->Update();
}

void BossBullet::DrawCollider()
{
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, {1,1,1,1});
#endif // _DEBUG
}

void BossBullet::OnCollision()
{
	if (!isActive_) {
		return;
	}

	//死亡フラグON
	isDead_ = true;
	//コライダーを無効にする
	collider_->ColliderOff();
}


