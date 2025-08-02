#include "BossManager.h"

BossManager* BossManager::GetInstance() {
	static BossManager instance;

	return &instance;
}

void BossManager::Initialize(BossType bosstype) {
	bosstype_ = bosstype;
}

void BossManager::SetBossType(BossType bosstype) {
	bosstype_ = bosstype;
}

void BossManager::SetTargetInfos(const std::vector<TargetInfo>& targets) {
	targetInfos_ = targets;
}

const std::vector<TargetInfo>& BossManager::GetTargetInfos() const {
	return targetInfos_;
}