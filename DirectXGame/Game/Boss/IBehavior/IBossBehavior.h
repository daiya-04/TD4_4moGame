#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include"FollowCamera/FollowCamera.h"
#include"WorldTransform.h"
#include"Boss/Bullet/types.h"
#include<optional>


//前方宣言
class Boss;

namespace DaiEngine {
	class Camera;
}

//パラメータ
struct BossParameters {

	//参照用本体ワールド
	const DaiEngine::WorldTransform* world;

	//プレイヤーワールド
	const DaiEngine::WorldTransform* playerWorld_;

	//カウント時間
	float currentSec = 0;
	Vector3 velocity_;
	Vector3 rotation_;

	float fieldY_= 0;

	//アニメーション変更名（””なら空）
	std::string setAnimeName_ = "";
	//ループアニメーションかどうか
	bool isLoopAnime_ = false;
	//アニメーションの速度倍率
	float animationLeverage_ = 1.0f;

	//弾タイプリクエスト
	std::optional<BulletType>bulletTypeRequest_ = std::nullopt;

	//状態リクエスト
	std::optional<int>behaviorRequest_ = 0;

	//カメラ状態リクエスト
	std::optional<FollowCamera::State>cameraBehaviorRequest_ = std::nullopt;

	//ボスの弾の数
	int currentBulletNum_ = 0;

	//警告円の生成
	bool SpawnDangerZone_ = false;

	//プレイヤー方向に移動フラグ
	bool isMoveToPlayer_ = false;

	//マップ端衝突フラグ
	bool isHitMapEdge_ = false;

	//プレイヤー方向を向くフラグ
	bool isLookAtPlayer_ = false;
	//プレイヤー方向ベクトル
	Vector3 lookAtPlayerVec_ = {0,0,-1};

	//死亡フラグ
	bool isDead_ = false;

};

int GetRandomInt(int min, int max);

//ボスの状態基底クラス
class IBossBehavior {

protected://**共通変数**//

	//カメラ
	static DaiEngine::Camera* camera_;

public://**パブリック変数**//

	//ボス
	BossParameters* param_;

	//デバッグ用ツリー
	GvariTree tree_;

public://パブリック関数**//

	/// <summary>
	/// ポインタをセット
	/// </summary>
	/// <param name="boss"></param>
	static void SetPointer(DaiEngine::Camera* camera) { camera_ = camera; }

	IBossBehavior()=default;
	virtual ~IBossBehavior() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};
};