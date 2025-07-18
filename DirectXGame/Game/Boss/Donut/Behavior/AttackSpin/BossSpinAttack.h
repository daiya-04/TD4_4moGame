#pragma once
#include"Boss/IBossAttack/IBossAttack.h"
#include"Collider/SphereCollider.h"
#include"SquareDangerZone/SquareDangerZone.h"

#include<memory>

class Boss2SpinAttack : public IBossAttack {

public://**パブリック関数**//

	Boss2SpinAttack(BossParameters* param);
	~Boss2SpinAttack();

	void Draw()override;

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

	void OnCollision(DaiEngine::Collider* collider);

private:

	bool isDrawZone_ = false;

	//移動方向
	Vector3 velo_;
	
	//実行回数
	int currentExecuteCount_ = 0;

private:

	std::unique_ptr<SquareDangerZone>dangerZone_;

	//コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_ = nullptr;
	
	//コライダーの半径
	float radius_ = 1.0f; 

	Vector4 color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);


	//実行回数
	int executeCount_ = 3;

	//ダウンタイム
	float downCount_ = 60.0f;


	//予備動作再生倍率
	float preActionRate_ = 1.0f;
	//実行動作再生倍率
	float actionRate_ = 1.0f;
	//ダウン動作再生倍率
	float downActionRate_ = 1.0f;

	//移動速度
	float speed_ = 1.0f;

};