#include "EffectManager.h"


EffectManager* EffectManager::GetInstance() {
	static EffectManager instance;

	return &instance;
}

void EffectManager::Init() {



}

void EffectManager::Update() {

	for (auto& [key, effect] : effects_) {
		effect->Update();
	}

}

void EffectManager::Draw(const DaiEngine::Camera& camera) {

	for (auto& [key, effect] : effects_) {
		effect->Draw(camera);
	}

}

void EffectManager::AddEffect(const std::string& fileName, const std::string& modelName) {

	//なければ追加
	if (effects_.find(fileName) == effects_.end()) {
		effects_[fileName] = std::make_unique<Effect>();
		effects_[fileName]->Init(fileName, modelName);
	}

}

void EffectManager::Start(const std::string& effectName, const Vector3& pos) {

	auto it = effects_.find(effectName);
	if (it != effects_.end()) {
		it->second->Start(pos);
	}

}

void EffectManager::Start(const std::string& effectName, const Vector3& pos, float angle) {

	auto it = effects_.find(effectName);
	if (it != effects_.end()) {
		it->second->Start(pos, angle);
	}

}

