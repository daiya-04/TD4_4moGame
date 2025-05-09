#pragma once
#include"Player/behavior/IPlayerBehavior.h"

//プレイヤーの移動関係
class PlayerMove : public IProtBehavior {
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

private://**プライベート変数**//

	//移動速度
	float moveSpped_ = 1.0f;

	Vector3 debugInputMove;

};