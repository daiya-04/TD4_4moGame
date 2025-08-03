#include "CupCakeFallAttack.h"
#include"ColliderManager.h"
#include "EffectManager.h"
#include "AudioManager.h"

CupCakeFallAttack::CupCakeFallAttack(BossParameters* parameters)
{
	param_ = parameters;

	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("jumpATK", *param_->world, colliderRadius_);
	collider_->ColliderOff();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {if (collider->GetTag() == "player")collider_->ColliderOff(); });

	
	dZoneParam_.world.Init();
	dZoneParam_.target = parameters->playerWorld_;
	dangerZone_ = std::make_unique<SingleDangerZone>(dZoneParam_);

	tree_.name_ = "jumpAttack";
	tree_.SetValue("attackCount", &attackCount_);
	tree_.SetValue("jumpHeight", &jumpHeight_);
	tree_.SetValue("jumpNum", &jumpNum_);
	tree_.SetValue("stopCount", &stopCount_);
	tree_.SetValue("colliderRadius", &colliderRadius_);

	GvariTree zoneTree;
	zoneTree.name_ = "zone";
	zoneTree.SetValue("radius", &dZoneParam_.maxRadius);
	zoneTree.SetValue("warningCount", &dZoneParam_.maxWarningCount);
	zoneTree.SetValue("finalWarningCount", &dZoneParam_.maxFinalWarningCount);
	zoneTree.SetValue("binking", &dZoneParam_.blinkingNum);

	tree_.SetTreeData(zoneTree);

	stampSE_ = DaiEngine::AudioManager::Load("SE/CapCakeDownAttack.mp3");
}

CupCakeFallAttack::~CupCakeFallAttack()
{
	//コライダー削除
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void CupCakeFallAttack::InitBehavior0()
{
	//ステリセット
	currentJumpNum_ = 0;
	countRequest_ = 1;
	collider_->SetRadius(colliderRadius_);
	dangerZone_->Init(dZoneParam_);
}

void CupCakeFallAttack::InitBehavior1()
{
	//ジャンプ前
	dangerZone_->Init(dZoneParam_);
}

void CupCakeFallAttack::InitBehavior2()
{
	//ジャンプ処理
	currentJumpNum_++;

	basePos = param_->world->translation_;

	//ターゲット座標取得
	targetPos = param_->playerWorld_->translation_;

	targetPos.y = param_->fieldY_;

	collider_->ColliderOff();

	param_->isFly_ = true;
	//
	isHit_ = false;
}

void CupCakeFallAttack::InitBehavior3()
{
	//ジャンプ後交直
	collider_->ColliderOff();
	param_->isFly_ = false;
}



void CupCakeFallAttack::UpdateBehavior1()
{
	param_->isLookAtPlayer_ = true;

	dangerZone_->Update();

	if (dangerZone_->GetIsDead()) {
		countRequest_ = 2;
	}
}

void CupCakeFallAttack::UpdateBehavior2()
{

	dangerZone_->Update();

	float t = param_->currentSec / attackCount_;

	if (t >= 0.5f && !isHit_) {
		collider_->ColliderOn();
		isHit_ = true;
	}

	Vector3 newpos = Lerp(t, basePos, targetPos);
	//放物線の高さを加える
	newpos.y += jumpHeight_ * (1.0f - std::abs(2.0f * t - 1.0f));

	//現在の座標との差分を求める
	newpos = newpos - param_->world->translation_;

	param_->velocity_ = newpos;

	if (param_->currentSec >= attackCount_) {
		if (currentJumpNum_ >= jumpNum_) {
			countRequest_ = 3;
			
		}
		else {
			param_->bulletTypeRequest_ = BulletType::Dive;
			param_->SpawnDangerZone_ = true;
			//前の状態に戻る
			countRequest_ = 1;
		}
		EffectManager::GetInstance()->Trigger("CapCakeStampEffect", param_->world->translation_ - Vector3(0.0f, 1.0f, 0.0f));
		stampSE_->Play();
		collider_->ColliderOff();
	}

	collider_->Update();
}

void CupCakeFallAttack::UpdateBehavior3()
{
	if (param_->currentSec >= stopCount_) {
		param_->behaviorRequest_=0;
	}
}

void CupCakeFallAttack::OffAttackCollider()
{
	collider_->ColliderOff();
}

void CupCakeFallAttack::Draw()
{
	dangerZone_->Draw();
}
