#include "FollowCamera.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include<memory>

FollowCamera::FollowCamera(DaiEngine::Camera* camera, const Vector3& targetTranslation)
{
	camera_ = camera;
	target_ = &targetTranslation;

	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("FollowCamera");

	std::string stateNames[] = { "None" ,"Follow" };

	gvg->SetMonitorValue("Debug", &isDebug_);
	gvg->SetValue("devOff", &devOffset_);
	gvg->SetValue("devRotate", &devRotate_);
	gvg->SetValue("maxEsingCount", &maxEsingCount_);
	gvg->SetValue("followSpd", &followSpd_);
	gvg->SetValue("followSpdMaxDistance", &followSpdMaxDistance_);
	gvg->SetValue("maxFollowSpdMultiply", &maxFollowSpdMultiply_);
	for (int i = 0; i < (int)State::Count; i++) {
		gvg->SetValue("offset_" + stateNames[i], &offset_[i]);
		gvg->SetValue("rotation_" + stateNames[i], &rotation_[i]);
	}


}

void FollowCamera::Update()
{
	Vector3 offset = targetOffset_;
	Vector3 rotation = targetRotation_;
	//遷移中の場合
	if (isEsing_) {
		esingCount_++;

		//割合計算
		float t = (float)esingCount_ / (float)maxEsingCount_;
		//割合に応じて補間
		offset = Lerp(t, preOffset_, targetOffset_);
		rotation = Lerp(t, preRotation_, targetRotation_);
		//遷移完了チェック
		if (t >= 1.0f) {
			//フラグをオフ
			isEsing_ = false;
			//遷移完了時に目標値を設定
			offset = targetOffset_;
			rotation = targetRotation_;
		}
	}
	else {
		if (state_ == State::Follow) {
			offset = *target_ + offset;
			//カメラの追従速度分だけ近づく
			CameraDelay(offset);
		}

	}

	if (isDebug_) {
		offset = devOffset_;
		rotation = devRotate_;
	}

	//座標と回転を渡す
	camera_->translation_ = offset;
	camera_->rotation_ = rotation;


}

void FollowCamera::SetState(State state)
{
	//ステート変更
	state_ = state;
	//カウントリセット
	esingCount_ = 0;


	isEsing_ = true;
	//過去情報と目標情報設定
	preOffset_ = camera_->translation_;
	preRotation_ = camera_->rotation_;
	//目標情報設定
	targetOffset_ = offset_[(int)state_];
	targetRotation_ = rotation_[(int)state_];

}

void FollowCamera::CameraDelay(Vector3& offset)
{

	//カメラに向かうベクトル取得
	Vector3 targetPos = offset - camera_->translation_;

	//追従速度より近くの場合はそのまま
	if (targetPos.Length() <= followSpd_) {
		return;
	}
	else {

		float length = targetPos.Length();
		if (length > followSpdMaxDistance_) {
			length = followSpdMaxDistance_;
		}

		float t = length / followSpdMaxDistance_;

		//追従速度分だけ近づく
		offset = camera_->translation_ + targetPos.Normalize() * (followSpd_ * maxFollowSpdMultiply_ * t);
	}


}
