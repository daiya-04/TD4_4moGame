#pragma once
#include"WorldTransform.h"
#include"SphereCollider.h"
#include"Boss/Bullet/types.h"
#include"Camera.h"
#include<memory>

struct BossBulletData {

	//行動タイプ
	BulletType type;

	//座標
	DaiEngine::WorldTransform world;
	//移動ベクトル
	Vector3 velocity = { 0,0,0 };
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

//ボスの球基底クラス
class IBossBullet {

public:

	//データ
	BossBulletData data_;

	//有効フラグ
	bool isActive_ = true;
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBossBullet()=default;
	~IBossBullet();

	void Init(const BossBulletData& data, DaiEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="collider"></param>
	virtual void OnCollision(DaiEngine::Collider* collider);

	/// <summary>
	/// ブロック接触時の追加処理
	/// </summary>
	virtual void AddOnCollisionBlock() {};

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateObject();

	/// <summary>
	/// コライダー描画
	/// </summary>
	void ColliderDraw();

	/// <summary>
	/// ブロックとの接触時判定
	/// </summary>
	void OnCollisionBlock();

	/// <summary>
	/// 死亡フラグ
	/// </summary>
	bool GetDead() { return isDead_; };

	BulletType GetType() { return data_.type; }

	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	const DaiEngine::WorldTransform& GetWorld()const { return data_.world; }

	/// <summary>
	/// コライダー半径
	/// </summary>
	/// <returns></returns>
	float GetColliderRadius() { return collider_->GetRadius(); }

private:

	//カメラポインタ
	DaiEngine::Camera* camera_ = nullptr;

	//円コライダー
	std::unique_ptr<DaiEngine::SphereCollider>collider_;

	//死亡フラグ
	bool isDead_ = false;
};