#pragma once
#include"GameObject.h"
#include"Boss/Behavior/IBossBehavior.h"
#include"BossBulletManager/BossBulletManager.h"
#include"Boss/DangerZoneManager/DangerZoneManager.h"
#include"SphereCollider.h"
#include"FollowCamera/FollowCamera.h"

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
	Boss(FollowCamera*camera);
	~Boss()=default;

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
	/// 警告エリア生成
	/// </summary>
	void SpawnDangerZone();

	/// <summary>
	/// 弾の発射処理
	/// </summary>
	void SpawnBullet(const DaiEngine::WorldTransform& pos);

	/// <summary>
	/// カメラの状態設定
	/// </summary>
	/// <param name="state"></param>
	void SetCameraState(FollowCamera::State state) { followCamera_->SetState(state); };

public://**セッター**//

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
	std::list<std::unique_ptr<BossBullet>>& GetBullets(){ return bulletManager_->GetBullets(); }

public://**状態

	//状態
	enum Behavior {
		Idle,
		Attack1,
		Attack2,
		Count
	}behavior_=Idle;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Idle;

private://**プライベート変数**//

	 FollowCamera* followCamera_ = nullptr;

	//状態
	std::vector<std::unique_ptr<IBossBehavior>>behaviors_;

	//プレイヤー座標
	const DaiEngine::WorldTransform* playerWorld_=nullptr;

	//弾マネージャ
	std::unique_ptr<BossBulletManager>bulletManager_ = nullptr;
	//警戒エリアマネージャ
	std::unique_ptr<DangerZoneManager>dangerZoneManager_ = nullptr;

	//コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_ = nullptr;

	//ワールド座標
	Vector3 position_{};



private://**パラメータ変数**//
	
	//コライダー半径
	float radius_ = 1.0f;

	//開始位置
	Vector3 startPosition_{};

	//オフセット位置
	Vector3 offsetPosition_{};

private://**デバッグ用変数**//
	
	//攻撃指定
	std::string debugBehavior_="None";

	std::vector<std::string> behaviorNames_ = {
	"None",
	"Idle",
	"Attack1",
	"Attack2"
	};
};