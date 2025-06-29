#pragma once
#include"../IBoss/IBoss.h"

class GingerbreadMan : public IBoss{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GingerbreadMan(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld);
	~GingerbreadMan() = default;

};