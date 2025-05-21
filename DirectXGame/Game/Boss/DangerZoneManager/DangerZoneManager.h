#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Boss/DangerZone/DangerZone.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"


//前方宣言
class Boss;

class DangerZoneManager :public InstancingGameObject {

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DangerZoneManager(Boss*boss);
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

private:

	GvariTree tree_;

	//ボスのポインタ
	Boss* boss_=nullptr;

	//プレイヤーのワールド座標取得
	std::list<std::unique_ptr<DangerZone>>dangerZones_;

private:

	//警告時間
	float warningTime_ = 0.5f;

	//警告円の高度誤差
	float warningHeight_ = 0.5f;
	//弾半径
	float radius_ = 1.0f;
};