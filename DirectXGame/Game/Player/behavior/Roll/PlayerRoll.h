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

	// ロール中の現在速度（XZ平面、Yは使わない）
	Vector3 currentVelo_;

private://**パラメータ**//

	

	// ロールの初速（開始時の勢い）
	float startSpeed_ = 10.0f;

	//速度減速量
	float decelRate = 0.05f;

	// 地形傾斜による重力加速度の倍率（地形が傾いていれば加速）
	float slopeGravity_ = 0.3f;

	// 傾斜がほぼ水平とみなす閾値（これ以下なら減速）
	float flatThreshold_ = 0.01f;



	//ロールのクールタイム
	float cooldownCount_ = 60.0f;

};