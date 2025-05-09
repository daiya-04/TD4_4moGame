#pragma once
#include"Player/behavior/Attacks/IPlayerAttack.h"

class PlayerAttackCombo3 : public IPlayerAttack {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttackCombo3();
	~PlayerAttackCombo3() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void InitPreliminaryAction() override;
	void InitAction() override;
	void InitRigorAction() override;
	/// <summary>
	/// 更新
	/// </summary>
	void UpdatePreliminaryAction() override;
	void UpdateAction() override;
	void UpdateRigorAction() override;

};