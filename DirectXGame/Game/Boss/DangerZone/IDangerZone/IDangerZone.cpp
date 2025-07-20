#include"IDangerZone.h"

void IDangerZone::Init(const DangerZoneParameters& param) {
	param_ = param;
}

void IDangerZone::Reset() {
	sec_ = 0;
	isDead_ = false;
	behavior_ = Behavior::Warning;
	isDraw_ = true;
	blinkingCount_ = 0;
}


void IDangerZone::UpdateObject() {
	//行列更新
	param_.world.UpdateMatrix();
}