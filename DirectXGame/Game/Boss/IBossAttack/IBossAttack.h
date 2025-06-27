#pragma once
#include"Boss/IBehavior/IBossBehavior.h"
#include<optional>

//ボスの攻撃の基底クラス
class IBossAttack : public IBossBehavior {

public://**パブリック関数**//


	IBossAttack();
	virtual ~IBossAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	void Draw()override {};

public://**状態**//

	/// <summary>
	/// 各状態の初期化関数
	/// </summary>
	virtual void InitBehavior0() = 0;
	virtual void InitBehavior1() = 0;
	virtual void InitBehavior2() = 0;
	virtual void InitBehavior3() = 0;
	virtual void InitBehavior4() = 0;

	/// <summary>
	/// 各状態の更新関数
	/// </summary>
	virtual void UpdateBehavior0() = 0;
	virtual void UpdateBehavior1() = 0;
	virtual void UpdateBehavior2() = 0;
	virtual void UpdateBehavior3() = 0;
	virtual void UpdateBehavior4() = 0;


	//関数テーブル
	static void (IBossAttack::* behaviorInit[])();
	static void (IBossAttack::* behaviorUpdate[])();

	//状態リクエスト
	std::optional<int>countRequest_ = std::nullopt;

public://**ぱ変数**//

	//現在の状態番号
	int behaviorCount_ = 0;
};