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
	BossBulletManager(Boss* boss);
	~BossBulletManager() = default;
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾を生成
	/// </summary>
	void SpawnBullet(const DaiEngine::WorldTransform& pos);

	/// <summary>
	/// ツリーの取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

	/// <summary>
	/// 弾丸のリストを取得
	/// </summary>
	/// <returns>弾のデータ群</returns>
	std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }

private:

	GvariTree tree_;

	//ボスのポインタ
	Boss* boss_ = nullptr;

	//弾データ群
	std::list<std::unique_ptr<BossBullet>>bullets_;

	//警告円用
	std::unique_ptr<InstancingGameObject>dangerZone_;

private:



	//弾の初期高度
	float bulletStartHeight_ = 10;

	//落下速度
	float fallSpeed_ = 10.0f;

	//弾半径
	float radius_ = 1.0f;

};