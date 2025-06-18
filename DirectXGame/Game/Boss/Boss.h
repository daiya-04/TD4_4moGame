#pragma once
#include"GameObject.h"
#include"Boss/Behavior/IBossBehavior.h"
#include"BossBulletManager/BossBulletManager.h"
#include"Boss/DangerZoneManager/DangerZoneManager.h"
#include"SphereCollider.h"
#include"FollowCamera/FollowCamera.h"

//セト
#include "Sprite.h"

#include<optional>
#include<string>

//ボスクラス
class Boss :public GameObject {

public://**パブリック変数**//

	//パラメータ
	struct Parameters {
		//カウント時間
		float currentSec = 0;
		Vector3 velocity_;
	}parameters_;


public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Boss(FollowCamera* camera);
	~Boss();

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
	/// 当たり判定処理
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(DaiEngine::Collider* collider);

	/// <summary>
	/// 警告エリア生成
	/// </summary>
	void SpawnDangerZone();

	/// <summary>
	/// 弾の発射処理
	/// </summary>
	void SpawnBullet(const DaiEngine::WorldTransform& pos);

	/// <summary>
	/// プレイヤー方向に移動
	/// </summary>
	void Move2Player();

	///セト
	void DrawUI();

private:

	void UIUpdate();

	///

public://**セッター**//

	/// <summary>
	/// カメラの状態設定
	/// </summary>
	/// <param name="state"></param>
	void SetCameraState(FollowCamera::State state) { followCamera_->SetState(state); };

	/// <summary>
	/// プレイヤーのワールド座標取得
	/// </summary>
	/// <param name="world"></param>
	void SetPlayerWorld(const DaiEngine::WorldTransform* world) { playerWorld_ = world; };

	/// <summary>
	/// プレイヤー方向を向く
	/// </summary>
	/// <returns></returns>
	Vector3 SetDirection2Player();

public://**ゲッター**//

	/// <summary>
	/// 弾データの取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bulletManager_->GetBullets(); }

	/// <summary>
	/// 死亡フラグチェック
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() const { return isDead_; }

public://**状態

	//状態
	enum Behavior {
		Idle,
		Attack1,
		Attack2,
		Count
	}behavior_ = Idle;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Idle;

private://**プライベート変数**//

	FollowCamera* followCamera_ = nullptr;

	//状態
	std::vector<std::unique_ptr<IBossBehavior>>behaviors_;

	//プレイヤー座標
	const DaiEngine::WorldTransform* playerWorld_ = nullptr;

	//弾マネージャ
	std::unique_ptr<BossBulletManager>bulletManager_ = nullptr;
	//警戒エリアマネージャ
	std::unique_ptr<DangerZoneManager>dangerZoneManager_ = nullptr;

	//コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_ = nullptr;

	//ワールド座標
	Vector3 position_{};

	///セト
	//UI
	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;
	std::unique_ptr<DaiEngine::Sprite> icon_;
	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

	///

private://**パラメータ変数**//

	//HP
	int HP_ = 100;

	//最大HP
	int maxHP_ = 100;

	//死亡フラグ
	bool isDead_ = false;

	//コライダー半径
	float radius_ = 1.0f;

	//開始位置
	Vector3 startPosition_{};

	//オフセット位置
	Vector3 offsetPosition_{};

	//移動速度
	float speed_ = 0.0f;

private://**ヒット時処理*//

		//ヒットフラグ
		bool isHit_ = true;

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

private://**デバッグ用変数**//

	//コライダーの色
	Vector4 colliderColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	//攻撃指定
	std::string debugBehavior_ = "None";

	std::vector<std::string> behaviorNames_ = {
	"None",
	"Idle",
	"Attack1",
	"Attack2"
	};

	//無敵フラグ
	bool isImmortal_ = false;

	//HP回復フラグ
	bool isHeal_ = false;
};