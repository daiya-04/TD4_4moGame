#include "IBossBullet.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

IBossBullet::~IBossBullet()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void IBossBullet::Init(const BossBulletData& data, DaiEngine::Camera* camera) {
	camera_ = camera;
	data_ = data;
	data_.world.scale_ = Vector3{ data_.radius,data_.radius,data_.radius };

	//コライダー初期化
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("敵の弾", data_.world, data_.colliderRadius);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });
	//コライダーを有効に
	collider_->ColliderOn();

	//Diveのみコライダー判定無し
	if (data.type == BulletType::Dive) {
		collider_->ColliderOff();
	}
}

void IBossBullet::ColliderDraw()
{
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, { 1,1,1,1 });
#endif // _DEBUG
}

void IBossBullet::UpdateObject() {
	//ワールド更新
	data_.world.UpdateMatrix();
	//コライダー更新
	collider_->Update();
}

void IBossBullet::OnCollision(DaiEngine::Collider* collider) {

	//有効ではないor同じ球の場合無視
	if (!isActive_||collider->GetTag()== "敵の弾") {
		return;
	}

	if (collider->GetTag() == "player"){
		collider_->ColliderOff();
		isDead_ = true;
	}
}

void IBossBullet::OnCollisionBlock()
{
	//有効ではない場合無視
	if (!isActive_) {
		return;
	}

	//死亡フラグON
	isDead_ = true;
	//コライダーを無効にする
	collider_->ColliderOff();

	//追加のブロック接触時判定
	AddOnCollisionBlock();
}
