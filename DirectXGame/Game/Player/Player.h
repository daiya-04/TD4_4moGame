#pragma once
#include"GameObject.h"
#include"Player/Input/PlayerInput.h"
#include"Player/behavior/IPlayerBehavior.h"
#include<optional>


struct ProtPlayerParameters {
	//体力
	int hp = 10;

	//ヒットフラグ
	bool isHit = true;

	//回避クールタイムカウント
	float currentRollCount = 0;

};


class Player :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	ProtPlayerParameters parameters_;

public://**パブリック変数**//

	//コンストラクタ
	Player();
	//デストラクタ
	~Player() = default;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	/// <summary>
	/// 座標をセットして更新
	/// </summary>
	/// <param name="translate">セットする座標</param>
	void SetWorldTranslate(const Vector3& translate);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void OnCollison();


public://**ゲッター**//

	/// <summary>
	/// 入力取得
	/// </summary>
	/// <returns></returns>
	PlayerInput* GetInput() { return input_.get(); };

public://**セッター

	/// <summary>
	/// 入力方向に体を向ける
	/// </summary>
	/// <returns>向きベクトル</returns>
	Vector3 SetBody2Input();

private://**プライベート関数**//

	/// <summary>
	/// 点滅処理
	/// </summary>
	void Tenmetu();

public://**状態**//

	//状態
	enum class Behavior{
		Move,	//移動
		Roll,	//回避
		Attack,	//攻撃
		Count	//状態の数
	}behaviorName_=Behavior::Move;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Move;

	//状態処理群
	std::vector<std::unique_ptr<IProtBehavior>>behaviors_;

private://**プライベート変数**//

	//プレイヤー入力クラス
	std::unique_ptr<PlayerInput>input_;

private://**ヒット時処理*//
	
	//無敵時間カウント
	float currentHitCount_ = 0;

	//点メス回数カウント
	int tenmetuCount_ = 0;

	//無敵時間
	float hitCount_ = 1.0f;

	//点滅回数
	int maxTenmetuNum_ = 3;
};