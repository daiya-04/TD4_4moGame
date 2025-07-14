#pragma once
#include"Boss/IBossAttack/IBossAttack.h"

class CupCakeWaveDive : public IBossAttack {

public:
	CupCakeWaveDive(BossParameters* parameters);
	~CupCakeWaveDive() = default;

public://**状態**//

	void InitBehavior0()override;
	void InitBehavior1()override;
	void InitBehavior2()override;
	void InitBehavior3()override;
	void InitBehavior4()override {};

	void UpdateBehavior0()override;
	void UpdateBehavior1()override;
	void UpdateBehavior2()override;
	void UpdateBehavior3()override;
	void UpdateBehavior4()override {};

private:

	//飛び上がる高さ
	float jumpHeight_ = 50.0f;

	//飛び上がる速度
	float jumpCount_ = 60.0f;

	//待機中の移動方向
	Vector3 waitVelo_;

	//落下待機時間
	float waitCount_ = 60.0f;


	//落下速度
	float fallSpd_ = 5.0f;

	//硬直時間
	float stopCount_ = 60.0f;


	//予備動作再生倍率
	float preActionRate_ = 1.0f;
	//実行動作再生倍率
	float actionRate_ = 1.0f;

};
