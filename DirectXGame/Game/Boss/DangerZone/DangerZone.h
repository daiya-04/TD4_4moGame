#pragma once
#include"WorldTransform.h"

struct DangerZoneParameters {
	DaiEngine::WorldTransform world;
	float maxRadius = 1.0f;
	float maxDeadSec = 0.5f;
};

class DangerZone{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DangerZone(const DangerZoneParameters& param);
	~DangerZone() = default;

	/// <summary>
	/// 再利用時のデータリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetDead() const { return isDead_; }

	/// <summary>
	/// パラメータをセット
	/// </summary>
	/// <param name="param"></param>
	void SetParameter(const DangerZoneParameters& param) { parameters_ = param; }

	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform& GetWorld() { return parameters_.world; }

private://**プライベート変数**//

	//パラメータ
	DangerZoneParameters parameters_;
	
	//時間
	float sec_ = 0;

	//死亡フラグ
	bool isDead_ = false;


};