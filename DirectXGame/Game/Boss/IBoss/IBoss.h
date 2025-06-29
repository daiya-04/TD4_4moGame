#pragma once
#include"GameObject.h"
#include"../../FollowCamera/FollowCamera.h"
#include"../IBehavior/IBossBehavior.h"
#include"SphereCollider.h"
#include"../BossBulletManager/BossBulletManager.h"
#include"../DangerZoneManager/DangerZoneManager.h"
#include"../UI/BossUI.h"
#include"../Blinking/Blinking.h"
#include"../../GlobalVariable/Tree/GlobalVariableTree.h"


//ボスの基底クラス、（状態と弾は継承先で生成
class IBoss :public GameObject{

public://**パブリック変数**//

	//ツリー
	GvariTree tree_;

	//パラメータ
	BossParameters parameters_;

	//状態
	std::vector<std::unique_ptr<IBossBehavior>>behaviors_;

	//状態
	enum BossBehavior {
		Idle,
		Attack1,
		Attack2,
		Count
	}behavior_ = Idle;

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBoss()=default;
	~IBoss();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="camera"></param>
	/// <param name="playerWorld"></param>
	void Init(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);

	/// <summary>
	/// マネージャの生成タイプ指定
	/// </summary>
	/// <param name="zoneType"></param>
	/// <param name="bulletType"></param>
	void SetManager(DangerZoneType zoneType,BulletType bulletType);

	/// <summary>
	/// 初期化
	/// </summary>
	void InitParameters();

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
	void DrawUI();

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 弾データの取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bulletManager_->GetBullets(); }

	/// <summary>
	/// ツリー取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }

private://**プライベート関数**//

	/// <summary>
/// パラメータフラグ更新
/// </summary>
	void ParameterFlagUpdate();

	/// <summary>
	/// ヒット処理
	/// </summary>
	/// <param name="collider"></param>
	virtual void OnCollision(DaiEngine::Collider* collider);

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

	/// <summary>
	/// カメラの状態設定
	/// </summary>
	/// <param name="state"></param>
	void SetCameraState(FollowCamera::State state) { followCamera_->SetState(state); };

	/// <summary>
	/// プレイヤー方向を向く
	/// </summary>
	/// <returns></returns>
	Vector3 SetDirection2Player();

	/// <summary>
	/// プレイヤー方向取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection2Player();

	/// <summary>
	/// 壁衝突判定
	/// </summary>
	void SetInField();


private://**プライベート変数**//

	//追従カメラ
	FollowCamera* followCamera_ = nullptr;

	//プレイヤー座標
	const DaiEngine::WorldTransform* playerWorld_ = nullptr;

	//コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_ = nullptr;


	//弾マネージャ
	std::unique_ptr<BossBulletManager>bulletManager_ = nullptr;
	//警戒エリアマネージャ
	std::unique_ptr<DangerZoneManager>dangerZoneManager_ = nullptr;

	//ボスUI
	std::unique_ptr<BossUI> ui_ = nullptr;

	//点滅処理
	std::unique_ptr<Blinking> blinking_ = nullptr;

	//ワールド座標
	Vector3 position_{};



private://**パラメータ変数**//

	//HP
	int HP_ = 100;
	//最大HP
	int maxHP_ = 100;
	//死亡フラグ
	bool isDead_ = false;
	//体コライダー半径
	float radius_ = 1.0f;
	//開始位置
	Vector3 startPosition_{};
	//オフセット位置
	Vector3 offsetPosition_{};
	//移動速度
	float speed_ = 0.0f;

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

	//MAPサイズ
	Vector2 mapArea_ = { 23.0f, 23.0f };
};