#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Boss/Bullet/BossBullet/BossBullet.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

class BossBulletManager : public InstancingGameObject {

public://**パブリック関数**//
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossBulletManager();
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
	/// 全ての弾データの削除
	/// </summary>
	void ClearAllBullets() { bullets_.clear(); }

	/// <summary>
	/// 弾を生成
	/// </summary>
	void SpawnBullet(const DaiEngine::WorldTransform& pos, BulletType type,const DaiEngine::WorldTransform& boss);

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

	//弾データ群
	std::list<std::unique_ptr<BossBullet>>bullets_;

	//警告円用
	std::unique_ptr<InstancingGameObject>dangerZone_;

private:

	//弾の初期高度
	float bulletStartHeight_ = 10;

	//落下速度
	float fallSpeed_ = 1.0f;

	//コライダー半径
	float colliderRadius_ = 1.0f;

	//放物線処理到着時間
	int arriveCount_ = 60;

	//放物線の高さ
	float parabolaHeight_ = 5.0f;

};