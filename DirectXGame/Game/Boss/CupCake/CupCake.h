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
		ShotBullet,			//弾を撃つ
		WaveDive,			//波を出す落下攻撃
		JumpAttack,
		Count
	};

};