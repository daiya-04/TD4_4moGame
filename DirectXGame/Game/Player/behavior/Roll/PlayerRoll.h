#pragma once
#include"Player/behavior/IPlayerBehavior.h"

//プレイヤーの回避処理
class PlayerRoll : public IProtBehavior {
public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerRoll();
	~PlayerRoll() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private://**プライベート変数**//

	//移動方向
	Vector3 velo_;

	//経過時間
	float currentCount_;

private://**パラメータ**//

	//回避時間
	float count_ = 1.0f;

	//回避速度
	float rollSpeed_ = 10.0f;

	//回避クールタイム
	float cooldowmCount_ = 1.0f;
};