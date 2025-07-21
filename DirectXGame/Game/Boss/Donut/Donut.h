#pragma once

#include"../IBoss/IBoss.h"

class Donut : public IBoss {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Donut(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);
	~Donut() = default;

private:

	enum BossBehavior {
		Idle	,			//アイドル
		Attack1,			//追従弾
		Attack2,			//タックル
		Dead,
		Count				//カウント
	};
};