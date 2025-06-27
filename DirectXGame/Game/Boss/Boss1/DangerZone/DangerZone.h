#pragma once
#include"WorldTransform.h"

enum class DangerZoneType {
	Normal,	//特になし
	Follow,	//追従
	Count	//数
};

struct DangerZoneParameters {
	//タイプ
	DangerZoneType type;
	DaiEngine::WorldTransform world;
	const Vector3* playerPos = nullptr;	//プレイヤーの位置
	float maxRadius = 1.0f;				//最大半径
	float maxWarningCount = 0.5f;		//警告時間
	float maxFinalWarningCount = 0.5f;		//最終警告時間時間
	int blinkingNum = 3;				//点滅回数

	float followSpd = 1.0f;			//追従速度
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
	bool GetIsDead() const { return isDead_; }

	bool GetIsDraw() const { return isDraw_; }

	/// <summary>
	/// パラメータをセット
	/// </summary>
	/// <param name="param"></param>
	void SetParameter(const DangerZoneParameters& param) { parameters_ = param; }

	void SetPlayerPos(const Vector3* pos) { pPos_ = pos; }

	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform& GetWorld() { return parameters_.world; }

private:

	/// <summary>
	/// 通常の更新
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// 追従時の更新
	/// </summary>
	void FollowUpdate();

private://**プライベート変数**//

	//状態
	enum class Behavior {

		Warning,
		FinalWarning,
		Count
	}behavior_ = Behavior::Warning;


	//パラメータ
	DangerZoneParameters parameters_;
	
	//時間
	float sec_ = 0;

	//死亡フラグ
	bool isDead_ = false;

	//描画フラグ
	bool isDraw_ = true;	

	//点滅カウント
	int blinkingCount_ = 0;

	const Vector3* pPos_ ;	//プレイヤーの位置
};