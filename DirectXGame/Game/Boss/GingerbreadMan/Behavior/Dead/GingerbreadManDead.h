#pragma once
#include"Boss/IBehavior/IBossBehavior.h"

class GingerbreadManDead : public IBossBehavior {

public:

	GingerbreadManDead(BossParameters* param);
	~GingerbreadManDead() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
private:

	float count_ = 60;

	float animeLeverage_ = 4.0f;

};