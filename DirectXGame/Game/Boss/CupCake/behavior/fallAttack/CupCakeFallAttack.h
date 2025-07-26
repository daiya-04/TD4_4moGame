#pragma once
#include"Boss/IBossAttack/IBossAttack.h"
#include"SphereCollider.h"
#include "Audio.h"

class CupCakeFallAttack : public IBossAttack {
public:
	CupCakeFallAttack(BossParameters* parameters);
	~CupCakeFallAttack();

public://**状態**//

	void InitBehavior0()override;
	void InitBehavior1()override;
	void InitBehavior2()override;
	void InitBehavior3()override;
	void InitBehavior4()override {};

	void UpdateBehavior0()override {};
	void UpdateBehavior1()override;
	void UpdateBehavior2()override;
	void UpdateBehavior3()override;
	void UpdateBehavior4()override {};

private:

	int currentJumpNum_ = 0;

	//元座標
	Vector3 basePos;

	Vector3 targetPos;

private:

	std::unique_ptr<DaiEngine::SphereCollider>collider_;

	float colliderRadius_ = 2.0f;

	//ジャンプ待機
	float waitCount_ = 60.0f;

	//ジャンプ攻撃時間
	float attackCount_ = 60.0f;

	// ジャンプ攻撃の高さ
	float jumpHeight_ = 20.0f;

	//ジャンプ回数
	int jumpNum_ = 3;

	//ジャンプ後交直
	float stopCount_ = 60.0f;

	bool isHit_ = false;

	DaiEngine::Audio* stampSE_ = nullptr;

};