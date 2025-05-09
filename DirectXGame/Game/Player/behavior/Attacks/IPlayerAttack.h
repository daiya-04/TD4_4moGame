#pragma once
#include"Player/behavior/IPlayerBehavior.h"
#include"Player/Input/PlayerInput.h"
#include<memory>
#include<optional>

class IPlayerAttack :public IProtBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IPlayerAttack();
	~IPlayerAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 次の攻撃フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsNextAttack() { return isAttack_; }

	/// <summary>
	/// 終了フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnd() { return isEnd_; }
public://**状態**//

	/// <summary>
	/// 各状態の初期化関数
	/// </summary>
	virtual void InitPreliminaryAction() = 0;
	virtual void InitAction() = 0;
	virtual void InitRigorAction() = 0;

	/// <summary>
	/// 各状態の更新関数
	/// </summary>
	virtual void UpdatePreliminaryAction() = 0;
	virtual void UpdateAction() = 0;
	virtual void UpdateRigorAction() = 0;

	//関数テーブル
	static void (IPlayerAttack::* behaviorInit[])();
	static void (IPlayerAttack::* behaviorUpdate[])();

	//状態リクエスト
	std::optional<int>behaviorRequest_ = std::nullopt;

private:


	std::unique_ptr<PlayerInput> input_ = nullptr;

	enum Behavior {
		PreliminaryAction,
		Action,
		RigorAction,
		Count
	};

	//時間
	float currentCount_ = 0;

	//現在の最大時間
	float nowCount_ = 0;

	//それぞれの最大時間
	float maxCounts_[Behavior::Count] = {1.0f};

	//状態番号
	int behavior_=0;

	//終了フラグ
	bool isEnd_ = false;

	//次の攻撃フラグ
	bool isAttack_ = false;
};