#pragma once
#include"Boss/DangerZone/IDangerZone/IDangerZone.h"

class FollowDangerZone : public IDangerZone {

public:

	FollowDangerZone(const DangerZoneParameters& param);
	~FollowDangerZone()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

};