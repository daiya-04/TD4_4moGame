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
	void Update(int currentHP, int maxHP);

	/// <summary>
	/// 描画
	/// </summary>
	void DrawUI();
private:

	///セト
//UI
	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;
	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

};