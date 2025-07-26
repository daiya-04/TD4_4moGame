#include "IBoss.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"
#include<memory>
#include <random>


IBoss::~IBoss()
{
	//コライダー削除
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void IBoss::Init(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld)
{

	//オブジェクト名設定
	GameObject::Init(objectName);

	//パラメータに本体ポインタ
	parameters_.world = world_;

	//カメラポインタ設定
	followCamera_ = camera;

	//プレイヤーポインタ
	playerWorld_ = playerWorld;

	parameters_.playerWorld_ = playerWorld_;

	//カメラポインタを設定
	IBossBehavior::SetPointer(camera->GetCamera());

	//マネージャ生成
	dangerZoneManager_ = std::make_unique<DangerZoneManager>();
	

	if (objectName != "CapCakeStandby") {
		bulletManager_ = std::make_unique<BossBulletManager>();
	}
	else {
		bulletManager_ = std::make_unique<BossBulletManager>(false);
	}
	bulletManager_->SetCamera(followCamera_->GetCamera());

	//点滅処理クラス生成
	blinking_ = std::make_unique<Blinking>();
	isDraw_ = &blinking_->GetIsDrawFlag();
	//UIクラス生成
	ui_ = std::make_unique<BossUI>();

	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("boss", *world_, radius_);
	collider_->ColliderOn();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });



	//汎用パラをあらかじめ設定
	tree_.SetMonitorValue("Immortal!!!!!!!!!!!!!!!!!", &isImmortal_);
	tree_.SetMonitorValue("HealHP", &isHeal_);
	tree_.SetMonitorValue("HP", &HP_);
	tree_.SetMonitorValue("currentCount", &parameters_.currentSec);
	
	tree_.SetMonitorValue("position", &world_->translation_);

	tree_.SetValue("MaxHP", &maxHP_);
	tree_.SetValue("Speed", &speed_);
	tree_.SetValue("Scale", &world_->scale_);
	tree_.SetValue("StartPos", &startPosition_);
	tree_.SetValue("OffsetPos", &offsetPosition_);
	tree_.SetValue("ColliderRadius", &radius_);
	tree_.SetValue("ColliderColor", &colliderColor_);
	tree_.SetValue("fieldSize", &mapArea_);

	tree_.SetTreeData(blinking_->GetTree());

	tree_.SetTreeData(dangerZoneManager_->GetTree());
	tree_.SetTreeData(bulletManager_->GetTree());

	GvariTree pDTree;
	pDTree.name_ = "damageFromPlayer";
	pDTree.SetValue("normal", &playerDamage_);
	pDTree.SetValue("spin", &playerSpinATKDamage_);

	tree_.SetTreeData(pDTree);
}

void IBoss::SetDebugBehaviorName(std::vector<std::string> names)
{
	tree_.SetMonitorCombo("setBehavior", &debugBehavior_, names);
	behaviorNames_ = names;
}

void IBoss::InitParameters()
{
	//開始位置設定
	position_ = startPosition_;
	//カメラ状態をフォローに設定
	SetCameraState(FollowCamera::State::Follow);
	//HP初期化
	HP_ = maxHP_;
	//コライダー半径設定
	collider_->SetRadius(radius_);
	//状態リクエスト
	parameters_.behaviorRequest_ = 0;
}

void IBoss::Update() {
	//移動量初期化
	parameters_.velocity_ = { 0,0,0 };

	//ボスの球数を取得
	parameters_.currentBulletNum_ = (int)bulletManager_->GetBullets().size()+(int)dangerZoneManager_->GetDangerZone().size();

#ifdef _DEBUG
	collider_->SetRadius(radius_);
	//回復フラグ処理
	if (isHeal_) {
		isHeal_ = false;
		HP_ = maxHP_;
	}
#endif // _DEBUG

	//リクエストがある場合
	if (parameters_.behaviorRequest_) {

#ifdef _DEBUG
		if (behaviorNames_.size() != 0&&debugBehavior_!=0) {
			//デバッグ時の攻撃指定
			//debugBehaviorが0の時は未指定
			parameters_.behaviorRequest_= debugBehavior_-1;
		}
#endif // _DEBUG

		//リクエストの値を渡す
		behavior_ = parameters_.behaviorRequest_.value();
		//リクエスト初期化
		parameters_.behaviorRequest_ = std::nullopt;
		parameters_.currentSec = 0;

		if(behaviors_.size() <= behavior_) {
			behavior_ = 0;
		}
		//状態初期化
		behaviors_[(int)behavior_]->Init();
	}

	//カウント増加
	parameters_.currentSec++;

	//状態更新
	behaviors_[(int)behavior_]->Update();

	//パラメータのフラグ更新
	ParameterFlagUpdate();

	//速度加算
	position_ += parameters_.velocity_;

	//オフセット位置加算
	world_->translation_ = position_ + offsetPosition_;

	//向き変更
	world_->rotation_ = parameters_.rotation_;

	//行列更新
	GameObject::Update();

	//点滅の更新
	blinking_->Update();

	//マネージャ更新
	dangerZoneManager_->Update();
	bulletManager_->Update();
	//コライダー更新
	collider_->Update();
	//UI更新
	ui_->Update(HP_, maxHP_);

}

void IBoss::ClearAllBulletAndZone()
{
	dangerZoneManager_->ClearAllDangerZone();
	bulletManager_->ClearAllBullets();
}

void IBoss::ParameterFlagUpdate() {
	//向きを変更する処理
	if (parameters_.isLookAtPlayer_) {
		parameters_.isLookAtPlayer_ = false;
		//プレイヤー方向を向く
		parameters_.lookAtPlayerVec_ = SetDirection2Player();
	}
	else {
		//プレイヤー方向を取得
		parameters_.lookAtPlayerVec_ = GetDirection2Player();
	}

	//移動処理
	if (parameters_.isMoveToPlayer_) {
		parameters_.isMoveToPlayer_ = false;
		Move2Player();
	}

	//アニメーションの変更
	if (parameters_.setAnimeName_ != "") {
		//アニメーション名が設定されている場合
		GameObject::SetAnimationName(parameters_.setAnimeName_, parameters_.isLoopAnime_);
		GameObject::SetAnimationLeverage(parameters_.animationLeverage_);
		//残りをクリア
		parameters_.setAnimeName_ = "";
		parameters_.isLoopAnime_ = true;
		parameters_.animationLeverage_ = 1.0f;
	}

	if (parameters_.cameraBehaviorRequest_) {
		//カメラの状態更新
		followCamera_->SetState(parameters_.cameraBehaviorRequest_.value());
		parameters_.cameraBehaviorRequest_ = std::nullopt;
	}

	if (parameters_.SpawnDangerZone_) {
		parameters_.SpawnDangerZone_ = false;
		SpawnDangerZone();
	}

	//ダメージゾーンの削除と弾出現
	dangerZoneManager_->GetDangerZone().remove_if([&](auto& data) {
		//死んでいる場合
		if (data->GetIsDead()) {
			//弾を出現させて削除
			SpawnBullet(data->GetWorld(),data->GetType());
			return true;
		}
		return false;
		});

	//フィールド内にいる場合
	SetInField();
}

void IBoss::Draw()
{
	//描画
	behaviors_[(int)behavior_]->Draw();

	//警告円の描画
	dangerZoneManager_->Draw();
	//弾の描画
	bulletManager_->Draw();

	//本体描画
	if (*isDraw_) {
		GameObject::Draw();
	}

	//円コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, colliderColor_);
#endif // _DEBUG
}

void IBoss::DrawUI()
{
	//UI描画
	ui_->Draw();
}

void IBoss::OnCollision(DaiEngine::Collider* collider)
{
	//プレイヤーの攻撃ならHP減少
	if (collider->GetTag() == "playerAttack") {
		HP_ -= playerDamage_;
		//カウント初期化
		blinking_->StartBlinking();

	} else if(collider->GetTag() == "playerSpin") {
		HP_ -= playerSpinATKDamage_;;
		//カウント初期化
		blinking_->StartBlinking();
	}

	//0なら死亡モーション
	if (HP_ <= 0) {
		//HPが0以下なら死亡
		//不死フラグが無効の場合
		if (!isImmortal_) {
			//体コライダーオフ
			collider_->ColliderOff();
			//全ての弾と警告円削除
			ClearAllBulletAndZone();

			//一番下の状態に変更（死亡状態
			parameters_.behaviorRequest_ = (int)behaviors_.size() - 1;
		}
	}
}

void IBoss::SpawnDangerZone() {
	if (parameters_.bulletTypeRequest_) {
		bulletType_ = parameters_.bulletTypeRequest_.value();
		parameters_.bulletTypeRequest_ = std::nullopt;
	}
	//プレイヤー座標取得
	dangerZoneManager_->SpawnDangerZone(playerWorld_->translation_,bulletType_);
}

void IBoss::SpawnBullet(const DaiEngine::WorldTransform& position,BulletType type) {
	//弾生成
	bulletManager_->SpawnBullet(position,type,*world_);
}

void IBoss::Move2Player() {
	//プレイヤー方向を見て向きベクトル取得
	Vector3 velo = SetDirection2Player();
	//正規化して速度を掛ける
	if (velo != Vector3(0, 0, 0)) {
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

float GetYRotatee(const Vector2& v) {
	Vector2 offset = { 0,1 };

	float dot = Dot(offset, v);

	float leng = offset.Length() * v.Length();

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}
	return angle;
}

Vector3 IBoss::SetDirection2Player() {
	//プレイヤー方向を取得
	Vector3 velocity = playerWorld_->GetWorldPos() - world_->GetWorldPos();
	//ｙの量を無視する
	velocity.y = 0.0f;
	if (velocity.Length() != 0) {
		//正規化
		velocity = velocity.Normalize();
	}

	//重なっていない場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		if (!isReverse_) {
			parameters_.rotation_.y = GetYRotatee({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
		}
		else {
			parameters_.rotation_.y = GetYRotatee({ velocity.x,velocity.z });
		}
	}

	return velocity;
}

Vector3 IBoss::GetDirection2Player() {
	//プレイヤー方向を取得
	Vector3 velocity = playerWorld_->GetWorldPos() - world_->GetWorldPos();
	//ｙの量を無視する
	velocity.y = 0.0f;
	if (velocity.Length() != 0) {
		//正規化
		velocity = velocity.Normalize();
	}
	return velocity;
}

void IBoss::SetInField() {
	if (position_.x - radius_ < -mapArea_.x) {
		position_.x = -mapArea_.x + radius_;
		parameters_.isHitMapEdge_ = true;
	}
	else if (position_.x + radius_ > mapArea_.x) {
		position_.x = mapArea_.x - radius_;
		parameters_.isHitMapEdge_ = true;
	}

	if (position_.z - radius_ < -mapArea_.y) {
		position_.z = -mapArea_.y + radius_;
		parameters_.isHitMapEdge_ = true;
	}
	else if (position_.z + radius_ > mapArea_.y) {
		position_.z = mapArea_.y - radius_;
		parameters_.isHitMapEdge_ = true;
	}
}
