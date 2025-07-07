#pragma once
#include"Boss/IBehavior/IBossBehavior.h"
#include<vector>

//ボスの待機状態
class CupCakeIdle : public IBossBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CupCakeIdle(BossParameters* param);
	~CupCakeIdle() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private://**パラメータ**//

	//時間
	float count_ = 60.0f;

	//攻撃保存
	std::vector<int>savedATK_;
};