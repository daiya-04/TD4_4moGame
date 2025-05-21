#include "Boss.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region 状態
#include"Boss/Behavior/Idle/BossIdle.h"
#include"Boss/Behavior/AreaAttack/BossAreaAttack.h"
#include"Boss/Behavior/WeaponRollAttack/BossWeaponRollAttack.h"
#pragma endregion


Boss::Boss()
{
	//オブジェクト生成
	GameObject::Init("Standing");

	IBossBehavior::SetBoss(this);

	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>();
	behaviors_[(size_t)Behavior::Attack1] = std::make_unique<BossAreaAttack>();
	behaviors_[(size_t)Behavior::Attack2] = std::make_unique<BossWeaponRollAttack>();


	std::unique_ptr<GlobalVariableGroup> gvg = std::make_unique<GlobalVariableGroup>("Boss");
	gvg->SetMonitorValue("currentCount", &parameters_.currentSec);

	//デバッグ用指定
	gvg->SetMonitorCombo("setBehavior", &debugBehavior_,behaviorNames_);

	for (auto& behavior : behaviors_) {
		if (!behavior)continue;
		gvg->SetTreeData(behavior->tree_);
	}

	//マネージャ生成
	dangerZoneManager_ = std::make_unique<DangerZoneManager>(this);
	bulletManager_ = std::make_unique<BossBulletManager>(this);

	gvg->SetTreeData(dangerZoneManager_->GetTree());
	gvg->SetTreeData(bulletManager_->GetTree());

}

void Boss::Update()
{

	//仮でプレイヤー方向に向き続ける
	SetDirection2Player();

	//リクエストがある場合
	if (behaviorRequest_) {

#ifdef _DEBUG
		//デバッグ時の攻撃指定
		if (debugBehavior_ == behaviorNames_[0]) {
			//何もなし
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

	//行列更新
	GameObject::Update();

	//マネージャ更新
	dangerZoneManager_->Update();
	bulletManager_->Update();
}

void Boss::Draw()
{
	//警告円の描画
	dangerZoneManager_->Draw();
	//弾の描画
	bulletManager_->Draw();

	//本体描画
	GameObject::Draw();
}

void Boss::SpawnDangerZone()
{
	////プレイヤー座標取得
	Vector3 pos = playerWorld_->GetWorldPos();
    dangerZoneManager_->SpawnDangerZone(pos);
}

void Boss::SpawnBullet(const Vector3&position)
{
	bulletManager_->SpawnBullet(position);
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


