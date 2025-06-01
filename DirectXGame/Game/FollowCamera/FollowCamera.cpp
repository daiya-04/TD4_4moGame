#include "FollowCamera.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include<memory>

FollowCamera::FollowCamera(DaiEngine::Camera* camera, const Vector3& targetTranslation)
{
	camera_ = camera;
	target_ = &targetTranslation;

	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("FollowCamera");
	gvg->SetValue("offset", &offset_);
	gvg->SetValue("rotation", &camera_->rotation_);
	gvg->SetMonitorValue("isFollow", &isFollow_);
}

void FollowCamera::Update()
{
	//追従ONの場合
	if (isFollow_) {
		//カメラの位置をターゲットの位置にオフセットを加えた位置にする
		camera_->translation_ = *target_ + offset_;
	}
	else {
		//カメラの位置をターゲットの位置にオフセットを加えた位置にする
		camera_->translation_ = offset_;
	}
}
