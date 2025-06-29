#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Boss/DangerZone/DangerZone.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

class DangerZoneManager :public InstancingGameObject {

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DangerZoneManager(DangerZoneType type);
	~DangerZoneManager() = default;
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 警告エリア生成
	/// </summary>
	void SpawnDangerZone(const Vector3&pos);

	/// <summary>
/// ツリーの取得
/// </summary>
/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

	/// <summary>
	/// 警告円データ群取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<DangerZone>>&GetDangerZone() { return dangerZones_; }

private:

	GvariTree tree_;

	//警告エリアのタイプ
	DangerZoneType type_;

	//プレイヤーのワールド座標取得
	std::list<std::unique_ptr<DangerZone>>dangerZones_;

private:

	//警告時間
	float warningTime_ = 60.0f;

	//最終警告時間
	float finalWarningTime_ = 30.0f;

	//点滅回数
	int blinkingNum_ = 3;

	//警告円の高度
	float warningHeight_ = 0.5f;
	//弾半径
	float radius_ = 1.0f;
	//色
	Vector4 color_ = Vector4(1, 0, 0, 1);

private://**追従式の場合のパラメータ
	
	//生成数
	int spawnNum_ = 4;

	//追従速度
	float followSpeed_ = 0.1f; 

	//プレイヤーとの距離
	float distance_ = 10.0f;
};