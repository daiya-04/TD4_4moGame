#pragma once
#include"../IBoss/IBoss.h"

class CupCake : public IBoss {

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CupCake(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);
	~CupCake() = default;

	enum Behavior {
		Idle,				//アイドル
		Count
	};

}