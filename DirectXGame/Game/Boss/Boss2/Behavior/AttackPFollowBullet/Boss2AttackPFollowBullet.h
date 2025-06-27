#pragma once
#include"Boss/IBossAttack/IBossAttack.h"


class Boss2AttackPFollowBullet : public IBossAttack {

public://**パブリック関数**//

	Boss2AttackPFollowBullet();
	~Boss2AttackPFollowBullet() = default;

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

	//移動方向
	Vector3 velo_;

private:

	//待機時間
	float waitTime_ = 60.0f;

	//実行時間
	float executeCount_ = 120.0f;



	//予備動作再生倍率
	float preActionRate_ = 1.0f;
	//実行動作再生倍率
	float actionRate_ = 1.0f;

};