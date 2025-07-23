#pragma once

#include "Boss/BossType/BossType.h"

class BossManager {
public:
	static BossManager* GetInstance();

	void Initialize(BossType bosstype);

	void SetBossType(BossType bosstype);

	const BossType GetBossType() { return bosstype_; }

private:
	BossManager() = default;
	~BossManager() = default;
	BossManager(const BossManager& obj) = default;
	BossManager& operator=(const BossManager& obj) = default;

	BossType bosstype_;
};