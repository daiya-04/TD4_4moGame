#pragma once
#include"../IBoss/IBoss.h"

//ボス生成マネージャ
class BossSpawnManager {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossSpawnManager(FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);
	~BossSpawnManager()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();

	/// <summary>
	/// 現在のボスの弾リストを取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bosses_[(int)bossType_]->GetBullets(); }

	/// <summary>
	/// 現在のボスの弾の種類を取得
	/// </summary>
	/// <returns></returns>
	BulletType GetBulletType() const { return bosses_[(int)bossType_]->GetBulletType(); }

	/// <summary>
	/// 全てのボスの死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetAllBossDead() const { return allBossDead_; }

	DaiEngine::WorldTransform& GetBossWorld() { return bosses_[int(bossType_)]->GetWorld(); }

private://**プライベート関数**//

	/// <summary>
	/// ボスの死亡チェック
	/// </summary>
	void CheckBossDead();

private://**プライベート変数**//

	//ボスの生成タイプ
	enum class BossType {
		GingerbreadMan,
		Donut,
		CupCake,
		Count
	}bossType_ = BossType::GingerbreadMan;


	//ボスのデータ群
	std::vector<std::unique_ptr<IBoss>> bosses_ ;

	//状態リクエスト
	std::optional<int>typeRequest_ = std::nullopt;


	//全てのボスの死亡フラグ
	bool allBossDead_ = false;

private://**デバッグ用**//

	//次のボスフラグ
	bool isNextBoss_ = false;

};