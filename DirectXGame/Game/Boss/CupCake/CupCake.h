#pragma once
#include"../IBoss/IBoss.h"

class CupCake : public IBoss {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CupCake(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);
	~CupCake() = default;

	enum class Behavior {
		Idle,				//アイドル
		Count
	};

};