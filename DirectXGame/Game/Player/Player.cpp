#include "Player.h"

#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"
#include "EffectManager.h"

#pragma region 状態クラス
#include"behavior/Entry/PlayerEntry.h"
#include"Player/behavior/Roll/PlayerRoll.h"
#include"Player/behavior/Move/PlayerMove.h"
#include"Player/behavior/AttackManager/PlayerAttackManager.h"
#include"Player/behavior/SpinAttack/SpinAttack.h"
#pragma endregion

#include<numbers>


Player::Player()
{
	//オブジェクト生成
	GameObject::Init("PlayerEntry");

	//入力クラス生成
	input_ = std::make_unique<PlayerInput>();

	//コライダークラス生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("player", *world_, radius_);
	collider_->ColliderOff();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) { OnCollison(collider); });
	//攻撃コライダー生成
	attackWorld_.Init();
	attackWorld_.parent_ = world_;
	attackCollider_ = std::make_unique<DaiEngine::SphereCollider>();
	attackCollider_->Init("playerAttack", attackWorld_, attackRadius_);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(attackCollider_.get());
	attackCollider_->SetStayCallback([this](DaiEngine::Collider* collider) { OnCollisionATKCollider(collider); });

	//プレイヤーポインタ設定
	IPlayerBehavior::SetPlayer(this);

	//状態の数指定
	behaviors_.resize((size_t)Behavior::Count);

	//生成
	behaviors_[(size_t)Behavior::Entry] = std::make_unique<PlayerEntry>();
	behaviors_[(size_t)Behavior::Move] = std::make_unique<PlayerMove>();
	behaviors_[(size_t)Behavior::Roll] = std::make_unique<PlayerRoll>();
	behaviors_[(size_t)Behavior::Attack] = std::make_unique<PlayerAttackManager>();
	behaviors_[(size_t)Behavior::SpinAttack] = std::make_unique<SpinAttack>();
	
	//描画フラグON
	SetDraw(false);

	ui_ = std::make_unique<PlayerUI>();

	///

#pragma region デバッグパラメータ設定
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Player");

	gvg->SetMonitorValue("Immortal!!!!!!!!!!!!!!!!!", &isImmortal_);
	gvg->SetMonitorValue("HealHP", &isHeal_);
	gvg->SetMonitorValue("HP", &parameters_.hp);
	gvg->SetMonitorValue("HitFlag", &parameters_.isHit);
	gvg->SetMonitorValue("RollCooldown", &parameters_.currentRollCount);
	gvg->SetValue("MaxHP", &maxHP_);
	gvg->SetValue("gravity", &gravity_);
	gvg->SetValue("OffsetPos", &offsetPos_);
	gvg->SetValue("Limitation", &limitationXZ_);
	gvg->SetValue("ColliderRadius", &radius_);
	gvg->SetValue("colliderColor", &colliderColor_);
	//全ての状態のツリーをセット
	for (auto& behavior : behaviors_) {
		if (behavior) {
			gvg->SetTreeData(behavior->tree_);
		}
	}

	//ヒット時の処理パラメータ設定
	GvariTree hitTree;
	hitTree.name_ = "Hit";
	hitTree.SetMonitorValue("IsHitFlag", &parameters_.isHit);
	hitTree.SetValue("MaxNoHitSec", &hitCount_);
	hitTree.SetValue("TenmetuNum", &maxBlinkingNum_);

	//攻撃用コライダー設定
	GvariTree attackColliderTree;
	attackColliderTree.name_ = "AttackCollider";
	attackColliderTree.SetValue("pos", &attackWorld_.translation_);
	attackColliderTree.SetValue("Radius", &attackRadius_);

	gvg->SetTreeData(hitTree);
	gvg->SetTreeData(attackColliderTree);
#pragma endregion	
}

Player::~Player()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(attackCollider_.get());
}

void Player::Init() {
	parameters_.hp = maxHP_;
	collider_->SetRadius(radius_);
	attackCollider_->SetRadius(attackRadius_);
}

void Player::Update()
{

#ifdef _DEBUG
	collider_->SetRadius(radius_);
	attackCollider_->SetRadius(attackRadius_);

	//回復フラグ処理
	if (isHeal_) {
		isHeal_ = false;
		parameters_.hp = maxHP_;
	}

#endif // DEBUG


	//移動量初期化
	parameters_.velocity = { 0,0,0 };

	//リクエストがある場合
	if (behaviorRequest_) {
		//リクエストの値を渡す
		behaviorName_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;
		//状態初期化
		behaviors_[(int)behaviorName_]->Init();
	}

	//回避のクールタイム更新
	parameters_.currentRollCount--;


	//もし時間が0以下なら0に
	if (parameters_.currentRollCount < 0)parameters_.currentRollCount = 0;

	//状態更新
	behaviors_[(int)behaviorName_]->Update();

	//落下
	parameters_.velocity.y -= gravity_;

	//座標更新
	UpdatePositionWithCollision();

	//制限チェック
	LimitationXZ();

	//オフセット分足してワールド座標更新
	world_->translation_ = position_ + offsetPos_;

	//点滅更新
	Blinking();

	//行列更新
	UpdateMatrix();

	//UI更新
	UIUpdate();

	//エフェクト更新
	//attackEffect_->UpdateObject();

}


///セト
void Player::UIUpdate() {


	ui_->Update(parameters_.hp, maxHP_);
}

void Player::UpdatePositionWithCollision()
{
	Vector3 nextPos = position_ + parameters_.velocity;

	if (field_ && !parameters_.isFlying) {
		Vector3 moveVec = nextPos - position_;
		float moveLen = moveVec.Length();

		if (moveLen > 0) {
			Vector3 moveDir = moveVec.Normalize();
			const float stepSize = 0.05f;
			float moved = 0.0f;

			Vector3 tempPos = position_;
			while (moved < moveLen) {
				float step = std::min(stepSize, moveLen - moved);
				tempPos += moveDir * step;

				Vector3 midPos = (position_ + tempPos) * 0.5f;

				if (field_->IsWalkable(tempPos) && field_->IsWalkable(midPos)) {
					position_ = tempPos;
					lastSafePos_ = position_;
				}
				else {
					position_ = lastSafePos_;
					parameters_.velocity = { 0, 0, 0 };
					break;
				}

				moved += step;
			}
		}

		if (!field_->IsWalkable(position_)) {
			if (auto warpPos = field_->FindNearestWalkable(position_); warpPos.has_value()) {
				position_ = warpPos.value();
				lastSafePos_ = position_;
			}
			else {
				position_ = lastSafePos_;
			}
			parameters_.velocity = { 0, 0, 0 };
		}
	}
	else {
		position_ = nextPos;
	}
}

void Player::DrawUI() {

	ui_->DrawUI();

}

///

void Player::UpdateOnField(float y)
{
	parameters_.blockY = y;
	//飛行中なら高さ修正しない
	if (parameters_.isFlying)return;

	//高さ修正
	if (world_->translation_.y < y) {
		world_->translation_.y = y;
		position_.y = world_->translation_.y - offsetPos_.y;
		//position_.y = y;
	}
	//行列更新
	UpdateMatrix();
}

void Player::Draw()
{
	//円コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_,colliderColor_);
	//ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(attackCollider_->GetShape()), *camera_,colliderColor_);
#endif // _DEBUG

	//描画
	if (isDraw_) {
		GameObject::Draw();
	}

	//エフェクト描画
	attackEffect_->Draw(*camera_);
}

void Player::UpdateMatrix() {
	//行列更新
	GameObject::Update();
	collider_->Update();
	attackWorld_.UpdateMatrix();
	attackCollider_->Update();
}

void Player::SetWorldTranslate(const Vector3& translate)
{
	world_->translation_ = translate;
	world_->UpdateMatrix();
}

void Player::OnCollison(DaiEngine::Collider* collider)
{
	//ボスコライダーの場合スキップ
	if (collider->GetTag() == "boss" || collider->GetTag() == "playerAttack" || !parameters_.isHit) {
		return;
	}


	//ヒットフラグOFF
	parameters_.isHit = false;

	//HP減少
	parameters_.hp--;

	if (parameters_.hp <= 0) {
		//HPが0以下ならゲームオーバー

		//不死フラグが無効の場合
		if (!isImmortal_) {
			isDead_ = true;
		}
	}

	//コライダーOFF
	collider_->ColliderOff();
}

void Player::OnCollisionATKCollider(DaiEngine::Collider* collider)
{
	//攻撃コライダーをOFF
	if (collider->GetTag() == "boss") {
		attackCollider_->ColliderOff();

		Matrix4x4 rotateMat_ = MakeRotateAxisAngle(Vector3(0.0f, 1.0f, 0.0f), GetWorld().rotation_.y);
		Vector3 offset = { 0.0f,0.0f,2.0f };
		offset = TransformNormal(offset, rotateMat_);

		EffectManager::GetInstance()->Trigger("BiteHitEffect", GetWorld().GetWorldPos() + offset);
	}
	
	

}

float GetYRotate(const Vector2& v) {
	Vector2 offset = { 0,1 };


	float dot = Dot(offset, v);

	float leng = offset.Length() * v.Length();

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}
	return angle;
}

Vector3 Player::Get2BossDirection()
{
	
	//ボスのワールド座標を取得
	Vector3 bossPos = bossWorld_->GetWorldPos();
	//プレイヤーのワールド座標を取得
	Vector3 playerPos = world_->GetWorldPos();
	//ボス方向のベクトルを計算
	Vector3 direction = bossPos - playerPos;
	return direction;

}

Vector3 Player::SetBody2Input()
{
	//入力を取得
	Vector3 velocity = input_->GetMoveInput();

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

	//入力がある場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		world_->rotation_.y = GetYRotate({ velocity.x,velocity.z });
	}

	return velocity;
}

void Player::SetAttackColliderActive(bool isActive)
{
	//isActiveがtrueなら攻撃を有効
	if (isActive) {
		attackCollider_->ColliderOn();
	}
	else {
		attackCollider_->ColliderOff();
	}
}

void Player::Blinking()
{
	if (!parameters_.isHit) {

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
			parameters_.isHit = true;
			isDraw_ = true;

			//カウント初期化
			currentHitCount_ = 0;
			//点滅回数初期化
			blinkingCount_ = 0;

			//コライダーON
			collider_->ColliderOn();
		}
	}
}

void Player::LimitationXZ()
{
	//XZ制限チェック
	if (position_.x <= -limitationXZ_.x) {
		position_.x = -limitationXZ_.x;
	}
	else if (position_.x >= limitationXZ_.x) {
		position_.x = limitationXZ_.x;
	}
	if (position_.z <= -limitationXZ_.y) {
		position_.z = -limitationXZ_.y;
	}
	else if (position_.z >= limitationXZ_.y) {
		position_.z = limitationXZ_.y;
	}
}
