#pragma once
#include"Player/behavior/IPlayerBehavior.h"

class PlayerEntry : public IPlayerBehavior {

public://**パブリック関数**//

	PlayerEntry();
	~PlayerEntry() override = default;

	void Init() override;
	void Update() override;

private://**パラメータ変数**//
	
	// 最大カウント
	float maxCount_ = 0.0f;

	// カウント
	float count_ = 0.0f; 

	//再生倍率
	float playRate_ = 1.0f;

};