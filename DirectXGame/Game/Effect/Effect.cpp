#include "Effect.h"

#include "ParticleManager.h"
#include "ModelManager.h"


void Effect::Init(const std::string& effectName, const std::string& modelName) {

	effect_ = ParticleManager::Load(effectName);

	if (!modelName.empty()) {
		obj_.reset(DaiEngine::SkinningObject::Create(DaiEngine::ModelManager::LoadGLTF(modelName)));
		obj_->SetAnimation(modelName, false);
		obj_->GetAnimation().SetAnimationSpeed(10.0f);
		obj_->worldTransform_.scale_ = { 2.0f,2.0f,2.0f };
	}

}

void Effect::Update() {

	if (!isEffect_) { return; }

	timer_ += kDeltaTime_;
	if (timer_ > kEffectTime_) {
		isEffect_ = false;
		timer_ = 0.0f;
	}


	for (auto& [group, particle] : effect_) {
		particle->Update();
	}
	if (obj_) { 
		obj_->Update(rotateMat_);
	}
}

void Effect::Draw(const DaiEngine::Camera& camera) {

	if (!isEffect_) { return; }

	if (obj_) {
		if (obj_->GetAnimation().IsPlaying()) {
			obj_->Draw(camera);
		}
	}

	for (auto& [group, particle] : effect_) {
		particle->Draw(camera, DaiEngine::BlendMode::kAlpha);
	}

}

void Effect::Start(const Vector3& pos) {

	isEffect_ = true;
	timer_ = 0.0f;

	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = pos;
		particle->Emit();
	}

}

void Effect::Start(const Vector3& pos, const float angle) {

	isEffect_ = true;
	timer_ = 0.0f;

	rotateMat_ = MakeRotateAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle);
	Vector3 offset = { 0.0f,0.0f,2.0f };
	offset = TransformNormal(offset, rotateMat_);


	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = pos + offset;
		particle->Emit();
	}

	if (obj_) { 
		obj_->worldTransform_.translation_ = pos + offset;
		obj_->GetAnimation().Start(false);
	}

}
