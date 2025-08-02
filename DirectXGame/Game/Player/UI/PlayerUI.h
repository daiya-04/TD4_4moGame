#pragma once

//セト
#include "Sprite.h"

#include<memory>

class PlayerUI {

public:

	PlayerUI();
	~PlayerUI() = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="currentHP"></param>
	/// <param name="maxHP"></param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawUI();

	void SetHp(int* hp, int maxHp) { hp_ = hp; maxHp_ = maxHp; }

	void SetCharge(float* charge, float maxCharge) { jumpCharge_ = charge; maxCharge_ = maxCharge; }
	

private:

	///セト
	//UI
	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;
	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

	int* hp_ = nullptr;
	int maxHp_ = 0;

	std::unique_ptr<DaiEngine::Sprite> jumpGauge_;
	float* jumpCharge_ = nullptr;
	float maxCharge_ = 0.0f;

};