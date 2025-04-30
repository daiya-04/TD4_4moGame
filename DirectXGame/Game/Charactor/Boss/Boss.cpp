#include "Boss.h"
#include <cmath>
#include <numbers>
#include "ModelManager.h"
#include "TextureManager.h"
#include "Easing.h"
#include "ModelManager.h"
#include "ShapesDraw.h"
#include "AnimationManager.h"
#include "ParticleManager.h"
#include "BossIdel.h"
#include "BossAttack.h"
#include "BossAppear.h"
#include "BossDead.h"
#include "BossMove.h"
#include "ColliderManager.h"
#include "Line.h"

#include "GroundFlare.h"

void Boss::Init(const std::vector<std::shared_ptr<DaiEngine::Model>>& models) {

	//アクション設定
	actionIndex_ = Action::Standing;
	
	//モデル関連の初期化
	BaseCharactor::Init(models);

	obj_.reset(DaiEngine::Object3d::Create(animationModels_[actionIndex_]));

	collider_->Init("Boss", obj_->worldTransform_, {});
	collider_->SetStayCallback([this](DaiEngine::Collider* other) {this->OnCollision(other); });
	collider_->ColliderOn();

	///エフェクト初期化
	effect_ = ParticleManager::Load("BossEnter");
	for (auto& [group, particle] : effect_) {
		particle->particleData_.isLoop_ = false;
	}
	deadEff_ = ParticleManager::Load("BossDeadEff");
	auraEff_ = ParticleManager::Load("BossAura");
	for (auto& [group, particle] : auraEff_) {
		particle->particleData_.isLoop_ = false;
	}
	///

	//状態設定
	ChangeBehavior("Appear");
	//攻撃設定
	attackType_ = AttackType::kElementBall;

	ChangeAuraColor();

	//HPの設定
	hp_.Init(DaiEngine::TextureManager::Load("Boss_HP.png"), { 390.0f,40.0f }, { 500.0f,10.0f });
	hp_.SetMaxHP(maxHp_);

}

void Boss::Update() {

	if (behaviorRequest_) {
		behavior_ = std::move(behaviorRequest_);
		behavior_->Init();
	}

	behavior_->Update();

	//行列更新
	obj_->worldTransform_.UpdateMatrixRotate(rotateMat_);
	UpdateCenterPos();
	//アニメーション再生
	animations_[actionIndex_].Play(animationModels_[actionIndex_]->rootNode_);
	obj_->worldTransform_.matWorld_ = animations_[actionIndex_].GetLocalMatrix() * obj_->worldTransform_.matWorld_;
	obj_->worldTransform_.WorldInverseTransposeMat_ = (obj_->worldTransform_.matWorld_.Inverse()).Transpose();

	BaseCharactor::Update();

	//エフェクト発生位置更新
	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = obj_->GetWorldPos();
		particle->particleData_.emitter_.translate.y = 0.01f;
		particle->Update();
	}
	for (auto& [group, particle] : deadEff_) {
		particle->Update();
	}
	for (auto& [group, particle] : auraEff_) {
		particle->particleData_.emitter_.translate = obj_->GetWorldPos();
		particle->Update();
	}

}

void Boss::UpdateCenterPos() {
	centerPos_ = {
		obj_->worldTransform_.matWorld_.m[3][0],
		obj_->worldTransform_.matWorld_.m[3][1],
		obj_->worldTransform_.matWorld_.m[3][2],
	};
}


void Boss::Draw(const DaiEngine::Camera& camera) {
	BaseCharactor::Draw(camera);

	obj_->Draw(camera);
}

void Boss::DrawParticle(const DaiEngine::Camera& camera) {
	for (auto& [group, particle] : effect_) {
		particle->Draw(camera);
	}
	for (auto& [group, particle] : deadEff_) {
		particle->Draw(camera);
	}
	for (auto& [group, particle] : auraEff_) {
		particle->Draw(camera);
	}
}

void Boss::OnCollision(DaiEngine::Collider* other) {

	if (other->GetTag() == "PlayerAttack") {
		hp_.TakeDamage();
	}
	
	//HPが0になったら...
	if (hp_.GetHP() <= 0) {
		isDead_ = true;
		ChangeBehavior("Dead");
	}
}

void Boss::ChangeBehavior(const std::string& behaviorName) {
	//行動とそれに対応するStateクラスの生成処理のマップ
	const std::unordered_map<std::string, std::function<std::unique_ptr<IBossBehavior>()>> behaviorTable{
		{"Idle", [this]() { return std::make_unique<BossIdle>(this); }},
		{"Attack", [this]() {return std::make_unique<BossAttack>(this); }},
		{"Appear", [this]() {return std::make_unique<BossAppear>(this); }},
		{"Dead", [this]() {return std::make_unique<BossDead>(this); }},
		{"Move", [this]() {return std::make_unique<BossMove>(this); }},
	};
	//検索
	auto nextBehavior = behaviorTable.find(behaviorName);
	if (nextBehavior != behaviorTable.end()) {
		//対応するStateクラスの生成
		behaviorRequest_ = nextBehavior->second();
	}

}

void Boss::SetData(const LevelData::ObjectData& data) {
	obj_->worldTransform_.translation_ = data.translation;
	obj_->worldTransform_.scale_ = data.scaling;

	BaseCharactor::SetData(data);

	//行列更新
	obj_->worldTransform_.UpdateMatrixRotate(rotateMat_);
	UpdateCenterPos();
}

void Boss::AppearEffectStart() {
	for (auto& [group, particle] : effect_) {
		particle->particleData_.isLoop_ = true;
	}
}

void Boss::AppearEffectEnd() {
	for (auto& [group, particle] : effect_) {
		particle->particleData_.isLoop_ = false;
	}
}

void Boss::DeadEffStart() {
	for (auto& [group, particle] : deadEff_) {
		particle->Emit();
		particle->particleData_.emitter_.translate = GetCenterPos();
	}
}

void Boss::AuraEffStart() {
	for (auto& [group, particle] : auraEff_) {
		particle->particleData_.isLoop_ = true;
	}
}

void Boss::AuraEffEnd() {
	for (auto& [group, particle] : auraEff_) {
		particle->particleData_.isLoop_ = false;
	}
}

void Boss::LookAtTarget() {
	Vector3 direction = target_->GetWorldPos().GetXZ() - obj_->GetWorldPos().GetXZ();
	SetDirection(direction);
}

void Boss::ChangeAuraColor() {
	for (auto& [group, particle] : auraEff_) {
		particle->particleData_.emitter_.color = auraColors_[attackType_];
	}
}
