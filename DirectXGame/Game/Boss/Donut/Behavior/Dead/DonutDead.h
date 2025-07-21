#pragma once
#include"Boss/IBehavior/IBossBehavior.h"

class DonutDead : public IBossBehavior {

public:

	DonutDead(BossParameters* param);
	~DonutDead() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
private:

	float count_ = 0;

	float animeLeverage_ = 1.0f;

};