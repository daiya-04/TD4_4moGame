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
		isTrigger_ = false;
		timer_ = 0.0f;
	}
	


	for (auto& [group, particle] : effect_) {
		if (particle->particleData_.isLoop_) {
			particle->particleData_.emitter_.translate = *parentPos_;
			timer_ = 0.0f;
		}
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

void Effect::Start(const Vector3* pos) {

	isEffect_ = true;

	parentPos_ = pos;

	for (auto& [group, particle] : effect_) {
		particle->particleData_.isLoop_ = true;
	}

}

//void Effect::Start(const Vector3* pos, const float angle) {
//
//	isEffect_ = true;
//	parentPos_ = pos;
//
//	rotateMat_ = MakeRotateAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle);
//
//
//	for (auto& [group, particle] : effect_) {
//		particle->particleData_.isLoop_ = true;
//	}
//
//	if (obj_) { 
//		obj_->GetAnimation().Start(false);
//	}
//
//}

void Effect::End() {

	parentPos_ = nullptr;

	for (auto& [group, particle] : effect_) {
		particle->particleData_.isLoop_ = false;
	}


}

void Effect::Trigger(const Vector3& pos) {
	isEffect_ = true;
	isTrigger_ = true;
	timer_ = 0.0f;

	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = pos;
		particle->Emit();
	}
}

void Effect::Trigger(const Vector3& pos, const float angle) {

	isEffect_ = true;
	isTrigger_ = true;
	timer_ = 0.0f;

	rotateMat_ = MakeRotateAxisAngle(Vector3(0.0f, 1.0f, 0.0f), angle);


	for (auto& [group, particle] : effect_) {
		particle->particleData_.emitter_.translate = pos;
		particle->Emit();
	}

	if (obj_) {
		obj_->worldTransform_.translation_ = pos;
		obj_->GetAnimation().Start(false);
	}

}
