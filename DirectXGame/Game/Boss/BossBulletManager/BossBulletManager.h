#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Boss/BossBullet/BossBullet.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//コンストラクタ
class Boss;

class BossBulletManager : public InstancingGameObject {

public://**パブリック関数**//
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossBulletManager(Boss*boss);
	~BossBulletManager() = default;
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 弾を生成
	/// </summary>
	void SpawnBullet(const Vector3& pos);

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

private:

	GvariTree tree_;

	//ボスのポインタ
	Boss* boss_=nullptr;

	//弾データ群
	std::list<std::unique_ptr<BossBullet>>bullets_;

private:



	//弾の初期高度
	float bulletStartHeight_ = 10;

	//落下速度
	float fallSpeed_ = 10.0f;

	//弾半径
	float radius_ = 1.0f;

};