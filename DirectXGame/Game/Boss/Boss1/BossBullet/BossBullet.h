#pragma once
#include"WorldTransform.h"
#include"SphereCollider.h"

#include<memory>

enum class BulletType {
	Normal,
	Follow,
	Count
};

struct BossBulletData {

	//行動タイプ
	BulletType type;

	//座標
	DaiEngine::WorldTransform world;
	//ベクトル
	Vector3 velocity = {0,0,0};
	//加速度
	Vector3 acceraletion;

	//警告円のワールド
	DaiEngine::WorldTransform warningWorld;

	//サイズ
	float radius = 1.0f;

	//コライダー半径
	float colliderRadius = 1.0f;
};

//ボスが出す一発の弾
class BossBullet {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタと初期化
	/// </summary>
	/// <param name="data"></param>
	BossBullet(const BossBulletData& data);
	~BossBullet();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 当たった時の処理
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 死亡フラグ
	/// </summary>
	bool GetDead() { return isDead_; };

	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	const DaiEngine::WorldTransform& GetWorld()const { return world_; }

	/// <summary>
	/// 警告円のワールド取得
	/// </summary>
	/// <returns></returns>
	const DaiEngine::WorldTransform& GetWarningWorld()const { return warningWorld_; }

	/// <summary>
	/// ワールドデータを取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform& GetWorld() { return world_; }

private://**パラメータ変数**//

	//ワールドデータ
	DaiEngine::WorldTransform world_;

	//円コライダー
	std::unique_ptr<DaiEngine::SphereCollider>collider_;

	//速度
	Vector3 velocity_;
	//加速度
	Vector3 acceleration_;
	
	//警告円のワールド
	DaiEngine::WorldTransform warningWorld_;

	//死亡フラグ
	bool isDead_ = false;
};