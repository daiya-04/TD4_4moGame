#pragma once
#include"Boss/Bullet/types.h"
#include"WorldTransform.h"

struct DangerZoneParameters {
	//タイプ
	BulletType type;
	DaiEngine::WorldTransform world;
	//const Vector3* playerPos = nullptr;	//プレイヤーの位置
	float maxRadius = 1.0f;				//最大半径
	float maxWarningCount = 60.0f;		//警告時間
	float maxFinalWarningCount = 30.0f;		//最終警告時間時間
	int blinkingNum = 3;				//点滅回数
	const DaiEngine::WorldTransform* target = nullptr;//ターゲット
	float followSpd = 1.0f;			//追従速度
};

class IDangerZone {

public:

	//状態
	enum class Behavior {
		Warning,
		FinalWarning,
		Count
	}behavior_ = Behavior::Warning;

	DangerZoneParameters param_;

	//プレイヤーの位置
	const Vector3* pPos_ = nullptr;

	//時間
	float sec_ = 0;

	//死亡フラグ
	bool isDead_ = false;

	//描画フラグ
	bool isDraw_ = true;


	//点滅カウント
	int blinkingCount_ = 0;
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IDangerZone()=default;
	virtual ~IDangerZone() = default;

	void Init(const DangerZoneParameters& param);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// オブジェクト更新
	/// </summary>
	void UpdateObject();

	/// <summary>
	/// プレイヤーの場所を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPlayerPos(const Vector3* pos) { pPos_ = pos; }

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() const { return isDead_; }

	/// <summary>
	/// 描画フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDraw() const { return isDraw_; }

	/// <summary>
	/// タイプ取得
	/// </summary>
	/// <returns></returns>
	BulletType GetType() const { return param_.type; }
	
	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform& GetWorld() { return param_.world; }

};