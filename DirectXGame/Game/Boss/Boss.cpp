#include "Boss.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

#pragma region 状態
#include"Boss/Behavior/Idle/BossIdle.h"
#include"Boss/Behavior/AreaAttack/BossAreaAttack.h"
#include"Boss/Behavior/WeaponRollAttack/BossWeaponRollAttack.h"
#pragma endregion


Boss::Boss(FollowCamera* camera)
{
	//オブジェクト生成
	GameObject::Init("GentlmanGuard");

	//カメラポインタ設定
	followCamera_ = camera;

	IBossBehavior::SetPointer(this,camera->GetCamera());

	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>();
	behaviors_[(size_t)Behavior::Attack1] = std::make_unique<BossAreaAttack>();
	behaviors_[(size_t)Behavior::Attack2] = std::make_unique<BossWeaponRollAttack>();

	//マネージャ生成
	dangerZoneManager_ = std::make_unique<DangerZoneManager>(this);
	bulletManager_ = std::make_unique<BossBulletManager>(this);
	
	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("boss", *world_, radius_);
	collider_->ColliderOn();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });

#pragma region デバッグパラメータセット
	std::unique_ptr<GlobalVariableGroup> gvg = std::make_unique<GlobalVariableGroup>("Boss");
	
	gvg->SetMonitorValue("Immortal!!!!!!!!!!!!!!!!!", &isImmortal_);
	gvg->SetMonitorValue("HealHP", &isHeal_);
	gvg->SetMonitorValue("HP", &HP_);
	
	gvg->SetMonitorValue("currentCount", &parameters_.currentSec);
	//デバッグ用指定
	gvg->SetMonitorCombo("setBehavior", &debugBehavior_, behaviorNames_);

	for (auto& behavior : behaviors_) {
		if (!behavior)continue;
		gvg->SetTreeData(behavior->tree_);
	}

	gvg->SetTreeData(dangerZoneManager_->GetTree());
	gvg->SetTreeData(bulletManager_->GetTree());
	gvg->SetValue("MaxHP", &maxHP_);
	gvg->SetValue("Speed", &speed_);
	gvg->SetValue("Scale", &world_->scale_);
	gvg->SetValue("StartPos", &startPosition_);
	gvg->SetValue("OffsetPos", &offsetPosition_);
#pragma endregion
}

void Boss::Initialize() {
	position_ = startPosition_;
	SetCameraState(FollowCamera::State::Follow);
	HP_ = maxHP_;
}

void Boss::Update()
{
	//移動量初期化
	parameters_.velocity_ = {0,0,0};

	//仮でプレイヤー方向に向き続ける
	SetDirection2Player();

#ifdef _DEBUG
	//回復フラグ処理
	if (isHeal_) {
		isHeal_ = false;
		HP_ = maxHP_;
	}
#endif // _DEBUG


	//リクエストがある場合
	if (behaviorRequest_) {

#ifdef _DEBUG
		//デバッグ時の攻撃指定
		if (debugBehavior_ == behaviorNames_[0]) {
			//指定なし
		}
		else if (debugBehavior_ == behaviorNames_[1]) {
			//待機
			behaviorRequest_ = Behavior::Idle;
		}
		else if (debugBehavior_ == behaviorNames_[2]) {
			//攻撃1
			behaviorRequest_ = Behavior::Attack1;
		}
		else {
			//攻撃2
			behaviorRequest_ = Behavior::Attack2;
		}
#endif // _DEBUG

		//リクエストの値を渡す
		behavior_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;
		parameters_.currentSec = 0;
		//状態初期化
		behaviors_[(int)behavior_]->Init();
	}

	//カウント増加
	parameters_.currentSec ++;

	//状態更新
	behaviors_[(int)behavior_]->Update();

	//速度加算
	position_ += parameters_.velocity_;

	//オフセット位置加算
	world_->translation_ = position_+offsetPosition_;

	//行列更新
	GameObject::Update();

	//マネージャ更新
	dangerZoneManager_->Update();
	bulletManager_->Update();
	collider_->Update();
}

void Boss::Draw()
{
	//警告円の描画
	dangerZoneManager_->Draw();
	//弾の描画
	bulletManager_->Draw();

	//描画
	behaviors_[(int)behavior_]->Draw();

	//本体描画
	GameObject::Draw();

	//円コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_);
#endif // _DEBUG

}

void Boss::OnCollision(DaiEngine::Collider* collider)
{
	//プレイヤーの攻撃ならHP減少
	if (collider->GetTag() == "playerAttack") {
		HP_--;
	}


	if(HP_ <= 0) {
		//HPが0以下なら死亡

		//不死フラグが無効の場合
		if (!isImmortal_) {
			isDead_ = true;
		}
	}
}

void Boss::SpawnDangerZone()
{
	////プレイヤー座標取得
	Vector3 pos = playerWorld_->GetWorldPos();
    dangerZoneManager_->SpawnDangerZone(pos);
}

void Boss::SpawnBullet(const DaiEngine::WorldTransform&position)
{
	bulletManager_->SpawnBullet(position);
}

void Boss::Move2Player()
{
	//プレイヤー方向を見て向きベクトル取得
	Vector3 velo = SetDirection2Player();
	//正規化して速度を掛ける
	if(velo!=Vector3(0, 0, 0)) {
		//プレイヤー方向に向ける
		velo = velo.Normalize() * speed_;
	}
	else {
		//プレイヤー方向がない場合は止まる
		velo = Vector3(0, 0, 0);
	}

	//ワールド座標に加算
	position_ += velo;

}

float GetYRotation(const Vector2& v) {
	Vector2 offset = { 0,1 };

	float dot = Dot(offset, v);

	float leng = offset.Length() * v.Length();

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}
	return angle;
}

Vector3 Boss::SetDirection2Player()
{
	//プレイヤー方向を取得
	Vector3 velocity = playerWorld_->GetWorldPos() - world_->GetWorldPos();

	//カメラ方向に向ける
	// 今はいらない（おそらくこの先も
	//velocity = TransformNormal(velocity, camera_->GetMainCamera().matWorld_);
	//ｙの量を無視する
	velocity.y = 0.0f;

	if (velocity.Length() != 0)
	{
		//正規化
		velocity = velocity.Normalize();
	}

	//重なっていない場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		world_->rotation_.y = GetYRotation({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
	}

	return velocity;
}


