#include "Player.h"

#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region 状態クラス
#include"Player/behavior/Roll/ProtPlayerRoll.h"
#include"Player/behavior/Move/ProtPlayerMove.h"
#include"Player/behavior/AttackManager/PlayerAttackManager.h"
#pragma endregion

#include<numbers>


Player::Player()
{
	//オブジェクト生成
	GameObject::Init("Standing");

	//入力クラス生成
	input_ = std::make_unique<PlayerInput>();

	//コライダークラス生成

	//プレイヤーポインタ設定
	IProtBehavior::SetPlayer(this);

	//状態の数指定
	behaviors_.resize((size_t)Behavior::Count);

	//生成
	behaviors_[(size_t)Behavior::Move] = std::make_unique<PlayerMove>();
	behaviors_[(size_t)Behavior::Roll] = std::make_unique<PlayerRoll>();
	behaviors_[(size_t)Behavior::Attack] = std::make_unique<PlayerAttackManager>();

	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Player");
	gvg->SetMonitorValue("HitFlag", &parameters_.isHit);
	gvg->SetMonitorValue("RollCooldown", &parameters_.currentRollCount);
	gvg->SetValue("HP", &parameters_.hp);

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
	hitTree.SetValue("TenmetuNum", &maxTenmetuNum_);
	gvg->SetTreeData(hitTree);
}

void Player::Update()
{
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
	parameters_.currentRollCount --;

	//もし時間が0以下なら0に
	if (parameters_.currentRollCount < 0)parameters_.currentRollCount = 0;

	//状態更新
	behaviors_[(int)behaviorName_]->Update();

	//点滅更新
	//Tenmetu();

	//行列更新
	GameObject::Update();
}

void Player::Draw()
{
	//描画
	GameObject::Draw();
}

void Player::SetWorldTranslate(const Vector3& translate)
{
	world_->translation_ = translate;
	world_->UpdateMatrix();
}

void Player::OnCollison()
{
	//ヒットフラグOFF
	parameters_.isHit = false;
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

Vector3 Player::SetBody2Input()
{
	//入力を取得
	Vector3 velocity = input_->GetMoveInput();

	//カメラ方向に向ける
	// 今はいらない（おそらくこの先も
	//velocity = TransformNormal(velocity, camera_->GetMainCamera().matWorld_);
	//ｙの量を無視する
	velocity.y = 0.0f;

	//正規化
	velocity = velocity.Normalize();

	//入力がある場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		//world_->rotation_.y = GetYRotate({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
	}

	return velocity;
}

void Player::Tenmetu()
{
	if (!parameters_.isHit) {

		currentHitCount_ ++;

		////時間内での点滅処理
		//if (currentHitCount_ >= (hitCount_/maxTenmetuNum_)*tenmetuCount_) {
		//	tenmetuCount_++;

		//	//透明度を変更
		//	if (model_->materialData_->color.w == 1.0f) {
		//		model_->materialData_->color.w = 0.1f;
		//	}
		//	else {
		//		model_->materialData_->color.w = 1.0f;
		//	}
		//}
		//	
		////時間経過で終了
		//if (currentHitCount_ >= hitCount_) {
		//	parameters_.isHit = true;
		//	model_->materialData_->color.w = 1.0f;

		//	//カウント初期化
		//	currentHitCount_ = 0;
		//	//点滅回数初期化
		//	tenmetuCount_ = 0;
		//}
	}
}
