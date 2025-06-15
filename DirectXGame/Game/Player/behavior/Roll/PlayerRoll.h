#pragma once
#include"Player/behavior/IPlayerBehavior.h"

//プレイヤーの回避処理
class PlayerRoll : public IPlayerBehavior {
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

	//初速度方向
	Vector3 currentVelo_;

	//経過時間
	float currentCount_ = 0.0f;

private://**パラメータ**//

	//初速度
	float startSpeed_ = 10.0f;

	//初速度最大減速時間
	float maxCount_ = 60.0f;

	//ロールクールタイム
	float cooldownCount_ = 60;
};