#pragma once
#include"../IBoss/IBoss.h"

#include "Boss/BossManager/BossManager.h"

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
	void Initialize(BossType bosstype);

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
	std::list<std::unique_ptr<IBossBullet>>& GetBullets() { return bosses_[(int)bossType_]->GetBullets(); }

	/// <summary>
	/// 全てのボスの死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetAllBossDead() const { return allBossDead_; }

	DaiEngine::WorldTransform& GetBossWorld() { return bosses_[int(bossType_)]->GetWorld(); }

	/// <summary>
	/// ブロック上に設置
	/// </summary>
	/// <param name="y"></param>
	void SetOnField(float y);

	/// <summary>
	/// ボスが死んだ瞬間を検知
	/// </summary>
	bool IsBossJustDied() const { return bossJustDied_; }

private://**プライベート関数**//

	/// <summary>
	/// ボスの死亡チェック
	/// </summary>
	void CheckBossDead();

private://**プライベート変数**//

	FollowCamera* camera_ = nullptr;

	//ボスのデータ群
	std::vector<std::unique_ptr<IBoss>> bosses_ ;

	BossType bossType_ = BossType::GingerbreadMan;

	//状態リクエスト
	std::optional<int>typeRequest_ = std::nullopt;

	//ボス引継ぎフラグ
	bool changeBoss_ = false;

	//変更時間
	int changeCount_ = 60;

	int currentChangeCount_ = 0;

	bool bossJustDied_ = false;

	//全てのボスの死亡フラグ
	bool allBossDead_ = false;

private://**デバッグ用**//

	//次のボスフラグ
	bool isNextBoss_ = false;

	std::string bossNames_[(int)BossType::Count] = {
		"GentlmanGuard",
		"Donut",
		"CapCake"
	};

	std::string nowBossName_ = "";

	BossManager* bossManager_ = nullptr;
};