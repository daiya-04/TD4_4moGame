#include "EffectManager.h"


EffectManager* EffectManager::GetInstance() {
	static EffectManager instance;

	return &instance;
}

void EffectManager::Init() {



}

void EffectManager::Update() {

	for (auto& [key, effects] : effectPool_) {
		for (auto& effect : effects) {
			effect->Update();
		}
	}

}

void EffectManager::Draw(const DaiEngine::Camera& camera) {

	for (auto& [key, effects] : effectPool_) {
		for (auto& effect : effects) {
			effect->Draw(camera);
		}
	}

}

void EffectManager::AddEffect(const std::string& fileName, const std::string& modelName) {

	
	std::vector<std::unique_ptr<Effect>> pool;
	for (size_t i = 0; i < 20; ++i) {
		auto effect = std::make_unique<Effect>();
		effect->Init(fileName, modelName);
		pool.push_back(std::move(effect));
	}
	effectPool_[fileName] = std::move(pool);

}

void EffectManager::Start(const std::string& effectName, const Vector3* pos) {

	auto it = effectPool_.find(effectName);
	if (it != effectPool_.end()) {
		for (auto& effect : it->second) {
			if (!effect->IsEffect()) {
				effect->Start(pos);
				return;
			}
		}
	}

}

//void EffectManager::Start(const std::string& effectName, const Vector3& pos, float angle) {
//
//	auto it = effectPool_.find(effectName);
//	if (it != effectPool_.end()) {
//		for (auto& effect : it->second) {
//			if (!effect->IsEffect()) {
//				effect->Start(pos, angle);
//				return;
//			}
//		}
//	}
//
//}

void EffectManager::End(const std::string& effectName) {
	auto it = effectPool_.find(effectName);
	if (it != effectPool_.end()) {
		for (auto& effect : it->second) {
			if (effect->IsEffect()) {
				effect->End();
			}
		}
	}
}

void EffectManager::Trigger(const std::string& effectName, const Vector3& pos) {

	auto it = effectPool_.find(effectName);
	if (it != effectPool_.end()) {
		for (auto& effect : it->second) {
			if (!effect->IsEffect()) {
				effect->Trigger(pos);
				return;
			}
		}
	}

}

void EffectManager::Trigger(const std::string& effectName, const Vector3& pos, float angle) {

	auto it = effectPool_.find(effectName);
	if (it != effectPool_.end()) {
		for (auto& effect : it->second) {
			if (!effect->IsEffect()) {
				effect->Trigger(pos, angle);
				return;
			}
		}
	}

}
