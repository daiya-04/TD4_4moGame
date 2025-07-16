#pragma once
#include"WorldTransform.h"
#include"SphereCollider.h"
#include"Boss/Bullet/types.h"

#include<memory>



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

	//放物線用ボスワールド
	DaiEngine::WorldTransform bossWorld;

	//到着時間
	int arriveCount = 60;

	//放物線の高さ
	float parabolaHeight = 5.0f;
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
	/// コライダーとの接触判定
	/// </summary>
	/// <param name="collier"></param>
	//void OnCollisionCollider(DaiEngine::Collider* collier);

	/// <summary>
	/// 死亡フラグ
	/// </summary>
	bool GetDead() { return isDead_; };

	/// <summary>
	/// 有効フラグ
	/// </summary>
	/// <returns></returns>
	bool GetActive() { return isActive_; }

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

	/// <summary>
	/// 弾タイプの取得
	/// </summary>
	/// <returns></returns>
	BulletType GetType() const { return type_; }

private://**パラメータ変数**//

	//ワールドデータ
	DaiEngine::WorldTransform world_;

	//弾の種類
	BulletType type_ = BulletType::Fall;

	//円コライダー
	std::unique_ptr<DaiEngine::SphereCollider>collider_;

	//速度
	Vector3 velocity_;
	//加速度
	Vector3 acceleration_;
	
	//警告円のワールド
	DaiEngine::WorldTransform warningWorld_;

	//放物線用のボスワールド
	DaiEngine::WorldTransform bossWorld_;

	//放物線用の到着時間
	int arriveCount_ = 60;
	//カウント
	int currentCount_ = 0;
	//放物線の高さ
	float parabolaHeight_ = 5.0f;

	//死亡フラグ
	bool isDead_ = false;

	//有効フラグ
	bool isActive_ = true;
};