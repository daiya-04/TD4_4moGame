#include "IBoss.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

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

	IBossBehavior::SetPointer(camera->GetCamera());

	//behaviorsの生成は継承先で行う
	//マネージャの生成は継承先で行う

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
	tree_.SetMonitorCombo("setBehavior", &debugBehavior_, behaviorNames_);

	tree_.SetValue("MaxHP", &maxHP_);
	tree_.SetValue("Speed", &speed_);
	tree_.SetValue("Scale", &world_->scale_);
	tree_.SetValue("StartPos", &startPosition_);
	tree_.SetValue("OffsetPos", &offsetPosition_);
	tree_.SetValue("ColliderRadius", &radius_);
	tree_.SetValue("ColliderColor", &colliderColor_);
	tree_.SetValue("fieldSize", &mapArea_);

	tree_.SetTreeData(blinking_->GetTree());
}

void IBoss::SetManager(DangerZoneType zoneType, BulletType bulletType)
{
	//マネージャ生成
	dangerZoneManager_ = std::make_unique<DangerZoneManager>(zoneType);
	bulletManager_ = std::make_unique<BossBulletManager>(bulletType);

	tree_.SetTreeData(dangerZoneManager_->GetTree());
	tree_.SetTreeData(bulletManager_->GetTree());
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
		//デバッグ時の攻撃指定
		if (debugBehavior_ == behaviorNames_[0]) {
			//指定なし
		}
		else if (debugBehavior_ == behaviorNames_[1]) {
			//待機
			parameters_.behaviorRequest_ = 0;
		}
		else if (debugBehavior_ == behaviorNames_[2]) {
			//攻撃1
			parameters_.behaviorRequest_ = 1;
		}
		else {
			//攻撃2
			parameters_.behaviorRequest_ = 2;
		}
#endif // _DEBUG

		//リクエストの値を渡す
		behavior_ = parameters_.behaviorRequest_.value();
		//リクエスト初期化
		parameters_.behaviorRequest_ = std::nullopt;
		parameters_.currentSec = 0;
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
			SpawnBullet(data->GetWorld());
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
		HP_--;
		//カウント初期化
		blinking_->StartBlinking();
	}
	if (HP_ <= 0) {
		//HPが0以下なら死亡
		//不死フラグが無効の場合
		if (!isImmortal_) {
			isDead_ = true;
		}
	}
}

void IBoss::SpawnDangerZone() {
	//プレイヤー座標取得
	dangerZoneManager_->SpawnDangerZone(playerWorld_->translation_);
}

void IBoss::SpawnBullet(const DaiEngine::WorldTransform& position) {
	//弾生成
	bulletManager_->SpawnBullet(position);
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
		parameters_.rotation_.y = GetYRotatee({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
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