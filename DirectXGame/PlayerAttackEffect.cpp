#include "PlayerAttackEffect.h"

#include "ParticleManager.h"


void PlayerAttackEffect::Init() {

	effect_ = ParticleManager::Load("BiteEffect");

}

void PlayerAttackEffect::Update() {

	if (!isEffect_) { return; }

	timer_ += kDeltaTime_;
	if (timer_ > kEffectTime_) {
		isEffect_ = false;
		timer_ = 0.0f;
	}

	for (auto& [group, particle] : effect_) {
		particle->Update();
	}

}

void PlayerAttackEffect::Draw([[maybe_unused]] const DaiEngine::Camera& camera) {

	for (auto& [group, particle] : effect_) {
		//particle->Draw(camera, DaiEngine::BlendMode::kAlpha);
	}

}

void PlayerAttackEffect::Start(const Vector3& pos, const float angle) {

	isEffect_ = true;
	timer_ = 0.0f;

	rotateMat_ = MakeRotateAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle);
	Vector3 offset = { 0.0f,0.0f,2.0f };
	offset = TransformNormal(offset, rotateMat_);


	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = pos + offset;
		particle->Emit();
	}

}
