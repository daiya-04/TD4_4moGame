#pragma once
#include"GameObject.h"
#include"Player/Input/PlayerInput.h"
#include"UI/PlayerUI.h"
#include"Player/behavior/IPlayerBehavior.h"
#include"SphereCollider.h"
#include "PlayerAttackEffect.h"
#include<optional>



#include"Field/Field.h"

struct PlayerParameters {

	//移動量
	Vector3 velocity;

	//地面の傾き方向
	Vector3 grandNormal = {0,1,0};

	//傾きの威力
	float grandInfluence =0;

	//体力
	int hp = 10;

	//ヒットフラグ
	bool isHit = true;

	//回避クールタイムカウント
	float currentRollCount = 0;

	//飛行フラグ
	bool isFlying = false;

};


class Player :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	PlayerParameters parameters_;

public://**パブリック変数**//

	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// フィールドの高さに合わせる
	/// </summary>
	void UpdateOnField(float y);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 行列更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 座標をセットして更新
	/// </summary>
	/// <param name="translate">セットする座標</param>
	void SetWorldTranslate(const Vector3& translate);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void OnCollison(DaiEngine::Collider* collider);

	/// <summary>
	/// 攻撃コライダーの衝突処理
	/// </summary>
	void OnCollisionATKCollider(DaiEngine::Collider* collider);

	/// <summary>
	/// ワールド取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform& GetWorld() { return *world_; };

	///セト
	void DrawUI();

private:

	/// <summary>
	/// UI更新
	/// </summary>
	void UIUpdate();

	// プレイヤーが穴空いてる場所を移動できないように
	void UpdatePositionWithCollision();

public://**ゲッター**//

	/// <summary>
	/// 入力取得
	/// </summary>
	/// <returns></returns>
	PlayerInput* GetInput() { return input_.get(); };

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() const { return isDead_; };

	PlayerAttackEffect* GetAttackEffect() { return attackEffect_; }

	Field* GetField() { return field_; }

	/// <summary>
	/// ボス方向のベクトル取得
	/// </summary>
	/// <returns></returns>
	Vector3 Get2BossDirection();


public://**セッター

	/// <summary>
	/// 入力方向に体を向ける
	/// </summary>
	/// <returns>向きベクトル</returns>
	Vector3 SetBody2Input();

	// <summary>
	/// コライダーの有効化処理
	/// </summary>
	/// <param name="isActive"></param>
	void SetColliderActive() { return collider_->ColliderOn(); };

	/// <summary>
	/// 攻撃コライダーの有効化無効化処理
	/// </summary>
	/// <param name="isActive"></param>
	void SetAttackColliderActive(bool isActive);

	/// <summary>
	/// 描画フラグ変更
	/// </summary>
	/// <param name="isDraw"></param>
	void SetDraw(bool isDraw) { isDraw_ = isDraw; };

	/// <summary>
	/// ポジションを設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos) { position_ = pos; };

	/// <summary>
	/// フィールドを設定
	/// </summary>
	/// <param name="pos"></param>
	void SetField(Field* field) { field_ = field; }

	/// <summary>
	/// 地面の傾斜取得
	/// </summary>
	/// <param name="hill">地面の法線（上下を考慮しない）</param>
	/// <param name="inflence">影響度</param>
	void SetFieldNormalVec(const Vector3& normalVec) { parameters_.grandNormal = normalVec;}

	void SetAttackEffect(PlayerAttackEffect* attackEffect) { attackEffect_ = attackEffect; }

	/// <summary>
	/// ボスのワールド座標
	/// </summary>
	/// <param name="bossWorld"></param>
	void SetBossWorld(DaiEngine::WorldTransform* bossWorld) { bossWorld_ = bossWorld; }


private://**プライベート関数**//

	/// <summary>
	/// 点滅処理
	/// </summary>
	void Blinking();

	/// <summary>
	/// XZ軸の制限処理
	/// </summary>
	void LimitationXZ();

public://**状態**//

	//状態
	enum class Behavior {
		Entry,	//エントリー
		Move,	//移動
		Roll,	//回避
		Attack,	//攻撃
		SpinAttack,		//スピンアタック
		Count	//状態の数
	}behaviorName_ = Behavior::Move;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Entry;

	//状態処理群
	std::vector<std::unique_ptr<IPlayerBehavior>>behaviors_;

private://**プライベート変数**//

	//ボスのワールド座標
	const DaiEngine::WorldTransform* bossWorld_ = nullptr;

	//プレイヤー入力クラス
	std::unique_ptr<PlayerInput>input_;

	//UIクラス
	std::unique_ptr<PlayerUI>ui_;

	//円コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_;

	//攻撃用コライダー
	std::unique_ptr<DaiEngine::SphereCollider> attackCollider_;

	//攻撃コライダー位置
	DaiEngine::WorldTransform attackWorld_;

	//攻撃コライダー半径
	float attackRadius_ = 1.0f;

	//コライダー半径
	float radius_ = 1.0f;

	//オフセット座標
	Vector3 offsetPos_ = { 0,0,0 };

	//座標
	Vector3 position_ = { 0,0,0 };

	//死亡フラグ
	bool isDead_ = false;

	//最大HP
	int maxHP_ = 10;

	//重力
	float gravity_ = 0.1f;



	//攻撃エフェクト
	PlayerAttackEffect* attackEffect_ = nullptr;

	///
	Vector3 lastSafePos_ = { 0,0,0 };

private://**ヒット時処理*//

	//無敵時間カウント
	float currentHitCount_ = 0;

	//点滅回数カウント
	int blinkingCount_ = 0;

	//無敵時間
	float hitCount_ = 1.0f;

	//点滅回数
	int maxBlinkingNum_ = 3;

	//描画フラグ
	bool isDraw_ = true;

private://**行動制限**//

	//XZ軸の制限
	Vector2 limitationXZ_ = { 10,10 };

	Field* field_ = nullptr; // 外からセットする

private://**デバッグ用パラメータ**//

	//コライダーの色
	Vector4 colliderColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	//無敵フラグ
	bool isImmortal_ = false;

	//HP回復フラグ
	bool isHeal_ = false;

};