#pragma once
#include"Boss/IBossAttack/IBossAttack.h"

//プレイヤーに弾を落とす攻撃を行う
class BossAreaAttack : public IBossAttack {

public:

	BossAreaAttack();
	~BossAreaAttack() = default;

public://**状態**//

	void InitBehavior0()override;
	void InitBehavior1()override;
	void InitBehavior2()override {};
	void InitBehavior3()override {};
	void InitBehavior4()override {};

	void UpdateBehavior0()override;
	void UpdateBehavior1()override;
	void UpdateBehavior2()override {};
	void UpdateBehavior3()override {};
	void UpdateBehavior4()override {};

private:

	//現在の数
	int currentNum_ = 0;

private:

	//待機時間
	float waitTime_ = 1.0f;

	//実行時間
	float attackTime_ = 3.0f;

	//個数
	int numbers_ = 3;

	//予備動作再生倍率
	float preActionRate_ = 1.0f;
	//実行動作再生倍率
	float actionRate_ = 1.0f;
};