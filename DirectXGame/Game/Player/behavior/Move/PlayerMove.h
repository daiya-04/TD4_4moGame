#pragma once
#include"Player/behavior/IPlayerBehavior.h"

//プレイヤーの移動関係
class PlayerMove : public IPlayerBehavior {
public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerMove();
	~PlayerMove()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private:

	/// <summary>
	/// ジャンプ更新
	/// </summary>
	void JumpUpdate(Vector3& move);

	/// <summary>
	/// 状態変化処理
	/// </summary>
	void BehaviorChange();

	/// <summary>
	/// 移動量による処理
	/// </summary>
	void MoveVeloUpdate(const Vector3& move);

private://**プライベート変数**//

	//移動速度
	float moveSpped_ = 1.0f;

	Vector3 debugInputMove;

	//移動フラグ
	bool isMove_ = false;

	//ジャンプフラグ
	bool isJump_ = false;

	//初速度
	float stSpd_=10.0f;

	//減速量
	float gravity_ = 1.0f;

	//頂点での減速量
	float topDegree_ = 4.0f;

	float nowSpd_;

	Vector3 emitPos_{};

};