#pragma once
#include"Boss/Behavior/IBossBehavior.h"

//ボスの待機状態
class BossIdle : public IBossBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossIdle();
	~BossIdle()=default;

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
	float sec_ = 1.0f;

};