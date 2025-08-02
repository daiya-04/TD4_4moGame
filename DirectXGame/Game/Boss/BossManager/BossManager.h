#pragma once

#include "Boss/BossType/BossType.h"
#include"Field/Field.h"

class BossManager {
public:
	static BossManager* GetInstance();

	void Initialize(BossType bosstype);

	void SetBossType(BossType bosstype);

	const BossType GetBossType() { return bosstype_; }

	void SetTargetInfos(const std::vector<TargetInfo>& targets);
	const std::vector<TargetInfo>& GetTargetInfos() const;

private:
	BossManager() = default;
	~BossManager() = default;
	BossManager(const BossManager& obj) = default;
	BossManager& operator=(const BossManager& obj) = default;

	BossType bosstype_;

	std::vector<TargetInfo> targetInfos_;
};