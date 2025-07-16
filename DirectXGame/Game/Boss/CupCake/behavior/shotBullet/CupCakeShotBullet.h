#pragma once
#include"Boss/IBossAttack/IBossAttack.h"

class CupCakeShotBullet : public IBossAttack {

public:

	CupCakeShotBullet(BossParameters* parameters);
	~CupCakeShotBullet()=default;

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

	//待機時間
	float waitCount_ = 60.0f;

	//実行時間
	float attackCount_ = 60.0f;

};