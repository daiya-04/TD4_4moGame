#include "Boss2.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"
#include "TextureManager.h"

#pragma region 状態
#include"Boss/Boss1/Behavior/Idle/BossIdle.h"
#include"Behavior/Attack2P/Boss2Attack2Player.h"
#include"Behavior/AttackPFollowBullet/Boss2AttackPFollowBullet.h"
#pragma endregion


Boss2::Boss2(FollowCamera* camera)
{
	//オブジェクト生成
	GameObject::Init("DonutAttack1.5");

	parameters_.world = world_;

	//カメラポインタ設定
	followCamera_ = camera;

	IBossBehavior::SetPointer(&parameters_, camera->GetCamera());

	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>();
	behaviors_[(size_t)Behavior::Attack1] = std::make_unique<Boss2Attack2Player>();
	behaviors_[(size_t)Behavior::Attack2] = std::make_unique<Boss2AttackPFollowBullet>();

	//マネージャ生成
	dangerZoneManager_ = std::make_unique<DangerZoneManager>(DangerZoneType::Follow);
	bulletManager_ = std::make_unique<BossBulletManager>(BulletType::Follow);


	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("boss", *world_, radius_);
	collider_->ColliderOn();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });


	///セト
	hpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGauge.png"), {}));
	hpGauge_->SetAnchorpoint({ 0.0f,0.5f });
	hpGauge_->SetPosition({ 365.0f,60.0f });
	gaugeSize_ = hpGauge_->GetSize();

	hpFream_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), { 340.0f,60.0f }));
	hpFream_->SetAnchorpoint({ 0.0f,0.5f });

	icon_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("bossIcon1.png"), { 330.0f,60.0f }));
	icon_->SetScale(0.6f);


	///
#pragma region デバッグパラメータセット
	std::unique_ptr<GlobalVariableGroup> gvg = std::make_unique<GlobalVariableGroup>("Boss2");

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

	//ヒット時の処理パラメータ設定
	GvariTree hitTree;
	hitTree.name_ = "Hit";
	hitTree.SetMonitorValue("IsHitFlag", &isHit_);
	hitTree.SetValue("MaxNoHitSec", &hitCount_);
	hitTree.SetValue("TenmetuNum", &maxBlinkingNum_);

	gvg->SetTreeData(hitTree);


	gvg->SetValue("MaxHP", &maxHP_);
	gvg->SetValue("Speed", &speed_);
	gvg->SetValue("Scale", &world_->scale_);
	gvg->SetValue("StartPos", &startPosition_);
	gvg->SetValue("OffsetPos", &offsetPosition_);
	gvg->SetValue("ColliderRadius", &radius_);
	gvg->SetValue("ColliderColor", &colliderColor_);

	gvg->SetValue("fieldSize", &mapArea_);

	gvg->SetTreeData(dangerZoneManager_->GetTree());
	gvg->SetTreeData(bulletManager_->GetTree());

#pragma endregion
}

Boss2::~Boss2()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void Boss2::Initialize() {
	position_ = startPosition_;
	SetCameraState(FollowCamera::State::Follow);
	HP_ = maxHP_;
	collider_->SetRadius(radius_);
	parameters_.behaviorRequest_ = Behavior::Idle;
}

void Boss2::Update()
{
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
			parameters_.behaviorRequest_ = Behavior::Idle;
		}
		else if (debugBehavior_ == behaviorNames_[2]) {
			//攻撃1
			parameters_.behaviorRequest_ = Behavior::Attack1;
		}
		else {
			//攻撃2
			parameters_.behaviorRequest_ = Behavior::Attack2;
		}
#endif // _DEBUG

		//リクエストの値を渡す
		behavior_ = (Behavior)parameters_.behaviorRequest_.value();
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

	//点滅の更新
	BlinkingUpdate();

	//速度加算
	position_ += parameters_.velocity_;

	//オフセット位置加算
	world_->translation_ = position_ + offsetPosition_;

	//向き変更
	world_->rotation_ = parameters_.rotation_;

	//行列更新
	GameObject::Update();

	UIUpdate();

	//マネージャ更新
	dangerZoneManager_->Update();
	bulletManager_->Update();
	collider_->Update();
}

///セト

void Boss2::UIUpdate() {

	percent_ = static_cast<float>(HP_) / static_cast<float>(maxHP_);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({}, { gaugeSize_.x * curPer_, gaugeSize_.y });


}

void Boss2::ParameterFlagUpdate()
{
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

void Boss2::BlinkingUpdate()
{
#pragma region Blinking
	if (!isHit_) {

		currentHitCount_++;

		////時間内での点滅処理
		if (currentHitCount_ >= (hitCount_ / maxBlinkingNum_) * blinkingCount_) {
			blinkingCount_++;

			//透明度を変更
			if (isDraw_) {
				isDraw_ = false;
			}
			else {
				isDraw_ = true;
			}
		}

		//時間経過で終了
		if (currentHitCount_ >= hitCount_) {
			isHit_ = true;
			isDraw_ = true;

			//カウント初期化
			currentHitCount_ = 0;
			//点滅回数初期化
			blinkingCount_ = 0;
		}
	}
#pragma endregion
}

void Boss2::DrawUI() {

	hpFream_->Draw();
	hpGauge_->Draw();
	icon_->Draw();

}


///

void Boss2::Draw()
{

	//描画
	behaviors_[(int)behavior_]->Draw();

	//本体描画
	if (isDraw_) {
		GameObject::Draw();
	}

	//警告円の描画
	dangerZoneManager_->Draw();
	//弾の描画
	bulletManager_->Draw();

	//円コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, colliderColor_);
#endif // _DEBUG

}

void Boss2::OnCollision(DaiEngine::Collider* collider)
{

	//プレイヤーの攻撃ならHP減少
	if (collider->GetTag() == "playerAttack") {
		HP_--;
		isHit_ = false;
		//カウント初期化
		blinkingCount_ = 0;
	}


	if (HP_ <= 0) {
		//HPが0以下なら死亡

		//不死フラグが無効の場合
		if (!isImmortal_) {
			isDead_ = true;
		}
	}
}

void Boss2::SpawnDangerZone()
{


	////プレイヤー座標取得
	dangerZoneManager_->SpawnDangerZone(playerWorld_->translation_);
}

void Boss2::SpawnBullet(const DaiEngine::WorldTransform& position)
{
	bulletManager_->SpawnBullet(position);
}

void Boss2::Move2Player()
{
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

float GetYRotation2(const Vector2& v) {
	Vector2 offset = { 0,1 };

	float dot = Dot(offset, v);

	float leng = offset.Length() * v.Length();

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}
	return angle;
}

Vector3 Boss2::SetDirection2Player()
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
		parameters_.rotation_.y = GetYRotation2({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
	}

	return velocity;
}

void Boss2::SetInField()
{
	if (position_.x - radius_ < -mapArea_.x) {
		position_.x = -mapArea_.x + radius_;
		parameters_.isHitMapEdge_ = true;
	}
	else if (position_.x + radius_ > mapArea_.x) {
		position_.x = mapArea_.x - radius_;
		parameters_.isHitMapEdge_ = true;
	}

	if(position_.z - radius_ < -mapArea_.y) {
		position_.z = -mapArea_.y + radius_;
		parameters_.isHitMapEdge_ = true;
	}
	else if (position_.z + radius_ > mapArea_.y) {
		position_.z = mapArea_.y - radius_;
		parameters_.isHitMapEdge_ = true;
	}

}

Vector3 Boss2::GetDirection2Player()
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

	return velocity;
}

