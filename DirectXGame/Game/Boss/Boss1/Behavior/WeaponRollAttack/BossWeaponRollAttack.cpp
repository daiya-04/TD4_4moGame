#include "BossWeaponRollAttack.h"
#include"Boss/Boss1/Boss.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"
#include"Camera.h"

BossWeaponRollAttack::BossWeaponRollAttack()
{
	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("bossRollATK", *param_->world, param.maxRadius);
	collider_->ColliderOff();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollisionATK(collider); });

	param.world = *param_->world;
	dangerZone_ = std::make_unique<SingleDangerZone>(param);

	tree_.name_ = "weaponRollAttack";
	tree_.SetValue("waitCount", &waitTime_);
	tree_.SetValue("activeCount", &attackTime_);
	tree_.SetValue("afterCount", &endTime_);

	tree_.SetValue("rotateNum", &rotateDegree_);
	tree_.SetValue("attackRadius_", &param.maxRadius);
	tree_.SetValue("offsetZone", &offsetZone_);
}

BossWeaponRollAttack::~BossWeaponRollAttack()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void BossWeaponRollAttack::Draw()
{
	//警告円描画
	dangerZone_->Draw();

	//円コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), (*camera_));
#endif // _DEBUG
}

void BossWeaponRollAttack::OnCollisionATK(DaiEngine::Collider* collider)
{
	//プレイヤーに当たったときのみOFF
	if(collider->GetTag()=="player") {
		//コライダーをOFF
		collider_->ColliderOff();
	}
}

void BossWeaponRollAttack::InitBehavior0()
{
	//予備動作初期化
	param_->setAnimeName_ = "GentlmanAttackPosture2";
	param_->isLoopAnime_ = false;
	//警告円初期化
	param.world.translation_ = param_->world->GetWorldPos() + offsetZone_;
	param.maxWarningCount = waitTime_;
	dangerZone_->Init(param);
}

void BossWeaponRollAttack::InitBehavior1()
{
	//回転数保存
	minRoll_ = param_->rotation_.y;
	maxRoll_ = minRoll_ + rotateDegree_;
	//アニメーション再生
	param_->setAnimeName_ = "GentlmanAttack2";
	param_->isLoopAnime_ = false;
	//コライダーON
	collider_->ColliderOn();
	collider_->SetRadius(param.maxRadius);
	
}

void BossWeaponRollAttack::InitBehavior2()
{	
	//コライダーOFF
	collider_->ColliderOff();

	//警告円の描画をOFF
	dangerZone_->SetDraw(false);
}

void BossWeaponRollAttack::UpdateBehavior0()
{
	//警告円の描画
	dangerZone_->Update();
	//時間経過で次
	if (dangerZone_->GetIsDead()) {
		countRequest_ = 1;
	}
}

void BossWeaponRollAttack::UpdateBehavior1()
{
	//割合Tを計算
	float t = param_->currentSec / attackTime_;

	//回転量を変更
	param_->rotation_.y = Lerp(t, minRoll_, maxRoll_);

	//カウントチェック
	if (param_->currentSec >= attackTime_) {
		countRequest_ = 2;
	}

	//コライダー更新
	collider_->Update();
}

void BossWeaponRollAttack::UpdateBehavior2()
{
	//時間経過で終わり
	if (param_->currentSec >= endTime_) {
		param_->behaviorRequest_ = Boss::Behavior::Idle;
	}
}
