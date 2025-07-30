#pragma once
#include"IPlayerBehavior.h"

class PlayerDead : public IPlayerBehavior {

public:

	PlayerDead();
	~PlayerDead() = default;

	void Init() override;
	void Update() override;

private:

	//アニメーション名
	std::string animeName_ = "PlayerDown";

	//再生倍率
	float playRate_ = 1.0f;

	//カウント
	int maxCount_ = 120;

	int count_ = 0;
};