#pragma once
#include"Player/behavior/Attacks/IPlayerAttack.h"
#include"SphereCollider.h"
#include "Audio.h"

#include<memory>

class SpinAttack : public IPlayerAttack {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpinAttack();
	~SpinAttack() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	void InitPreliminaryAction() override;
	void InitAction() override;
	void InitRigorAction() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void UpdatePreliminaryAction() override;
	void UpdateAction() override;
	void UpdateRigorAction() override;


private:

	std::unique_ptr<DaiEngine::SphereCollider>collider_;

	float radius_ = 2.0f;

	//上昇速度
	float upSpeed_ = 1.1f;

	//落下速度
	float downSpeed_ = 0.01f;

	//攻撃をやめるボスとの距離
	float stopDistance_ = 0.1f; // ボスとの距離がこの値以下になったら攻撃をやめる

	DaiEngine::Audio* se_ = nullptr;

};