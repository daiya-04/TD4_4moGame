#pragma once
#include"Boss/DangerZone/IDangerZone/IDangerZone.h"

class QuickDangerZone : public IDangerZone {

public:
	//通常の警告円

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param"></param>
	QuickDangerZone(const DangerZoneParameters& param);
	~QuickDangerZone() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
};