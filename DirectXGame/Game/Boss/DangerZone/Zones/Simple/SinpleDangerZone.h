#pragma once
#include"Boss/DangerZone/IDangerZone/IDangerZone.h"

class SinpleDangerZone : public IDangerZone {

public:
	//通常の警告円

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param"></param>
	SinpleDangerZone(const DangerZoneParameters& param);
	~SinpleDangerZone()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
};