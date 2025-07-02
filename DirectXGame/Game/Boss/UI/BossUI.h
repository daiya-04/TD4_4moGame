#pragma once
#include "Sprite.h"
#include<memory>

class BossUI {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossUI();
	~BossUI()=default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="HP_"></param>
	/// <param name="maxHP"></param>
	void Update(const int HP_,const int maxHP);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//UI
	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;
	std::unique_ptr<DaiEngine::Sprite> icon_;
	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

};