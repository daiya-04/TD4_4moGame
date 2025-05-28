#pragma once
#include"Boss/Behavior/IBossAttack/IBossAttack.h"

//プレイヤーに弾を落とす攻撃を行う
class BossWeaponRollAttack : public IBossAttack {

public:

	BossWeaponRollAttack();
	~BossWeaponRollAttack() = default;

public://**状態**//

	void InitBehavior0()override;
	void InitBehavior1()override;
	void InitBehavior2()override;
	void InitBehavior3()override {};
	void InitBehavior4()override {};

	void UpdateBehavior0()override;
	void UpdateBehavior1()override;
	void UpdateBehavior2()override;
	void UpdateBehavior3()override {};
	void UpdateBehavior4()override {};

private:

	//現在の数
	int currentNum_ = 0;

	//最小度
	float minRoll_ = 0;
	//最大度
	float maxRoll_ = 0;

private:

	//待機時間
	float waitTime_ = 1.0f;

	//実行時間
	float attackTime_ = 3.0f;

	//硬直時間
	float endTime_ = 1.0f;

	//回転度合
	float rotateDegree_ = 360.0f;

};