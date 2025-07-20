#pragma once
#include"Player/behavior/IPlayerBehavior.h"
#include"Player/behavior/Attacks/IPlayerAttack.h"
#include "Audio.h"
#include<memory>

class PlayerAttackManager : public IPlayerBehavior {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttackManager();
	~PlayerAttackManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//状態
	enum Behavior {
		Attack1,
		Attack2,
		Attack3,
		Count
	}behavior_;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//攻撃データ
	std::vector<std::unique_ptr<IPlayerAttack>>attacks_;

	DaiEngine::Audio* se_ = nullptr;
};