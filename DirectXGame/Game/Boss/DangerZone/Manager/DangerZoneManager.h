#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Boss/DangerZone/IDangerZone/IDangerZone.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

class DangerZoneManager :public InstancingGameObject {

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DangerZoneManager();
	~DangerZoneManager() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 全ての危険ゾーンの削除
	/// </summary>
	void ClearAllDangerZone() { dangerZones_.clear(); }

	/// <summary>
	/// 警告エリア生成
	/// </summary>
	void SpawnDangerZone(const Vector3&pos,BulletType type);

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

	/// <summary>
	/// 警告円データ群取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<IDangerZone>>&GetDangerZone() { return dangerZones_; }

private://**パラメータを設定**//

	void SetParameters(DangerZoneParameters& param);

private:

	GvariTree tree_;

	//プレイヤーのワールド座標取得
	std::list<std::unique_ptr<IDangerZone>>dangerZones_;

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


	//追従速度
	float followSpeed_ = 0.1f; 

	//プレイヤーとの距離
	float distance_ = 10.0f;

private://**ばらまきの場合のパラメータ*//

	//生成数
	int spawnNum_ = 4;

	//ばらまき最小最大範囲
	Vector2 spawnRange_ = Vector2(1.0f, 3.0f);
};