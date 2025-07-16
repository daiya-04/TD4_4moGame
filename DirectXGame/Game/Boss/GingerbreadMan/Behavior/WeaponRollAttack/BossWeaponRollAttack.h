#pragma once
#include"Boss/IBossAttack/IBossAttack.h"
#include"Boss/SingleDangerZone/SingleDangerZone.h"
#include"SphereCollider.h"
#include<memory>

class GingerbreadMan;

//プレイヤーに弾を落とす攻撃を行う
class BossWeaponRollAttack : public IBossAttack {

public://**パブリック関数**//

	BossWeaponRollAttack(BossParameters* param);
	~BossWeaponRollAttack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="collider"></param>
	void OnCollisionATK(DaiEngine::Collider* collider);

	void SetGingerbreadMan(GingerbreadMan* gingerbreadMan) { gingerbreadMan_ = gingerbreadMan; }

public://**状態関数**//

	void InitBehavior0()override;
	void InitBehavior1()override;
	void InitBehavior2()override;
	void InitBehavior3()override {};
	void InitBehavior4()override {};

	void UpdateBehavior0()override;
	void UpdateBehavior1()override;
	void UpdateBehavior2()override;
	void UpdateBehavior3()override {};
	void UpdateBehavior4()override {};


private://**プライベート変数**//

	//現在の数
	int currentNum_ = 0;

	//最小度
	float minRoll_ = 0;
	//最大度
	float maxRoll_ = 0;

	//コライダー
	std::unique_ptr<DaiEngine::SphereCollider> collider_ ;

	//警告円
	std::unique_ptr<SingleDangerZone> dangerZone_;

	GingerbreadMan* gingerbreadMan_ = nullptr;

private://**パラメータ変数**//

	//待機時間
	float waitCount_ = 60.0f;

	//実行時間
	float attackCount_ = 60.0f;

	//硬直時間
	float endCount_ = 60.0f;

	//回転度合
	float rotateDegree_ = 360.0f;

	//警戒エリアのパラメータ
	DangerZoneParameters param;


	//円のオフセット
	Vector3 offsetZone_ = Vector3(0.0f, 0.0f, 0.0f);
};