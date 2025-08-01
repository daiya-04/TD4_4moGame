#include "SquareDangerZone.h"

SquareDangerZone::SquareDangerZone(const DaiEngine::WorldTransform* world)
{
	//オブジェクト生成
	object_ = std::make_unique<InstancingGameObject>();
	object_->Init("Square", 1);

	world_.Init();
	world_.parent_ = world;

	tree_.name_ = "SquareDangerZone";
	tree_.SetValue("maxScale",&maxScale_);
	tree_.SetValue("warningCount", &warningCount_);
	tree_.SetValue("finalWarningCount", &finalWarningCount_);
	tree_.SetValue("color", &color_);
}

void SquareDangerZone::Init() {
	isEnd_ = false;
	count_ = 0;
	state_ = State::WARNING;
	isDraw_ = true;
}

void SquareDangerZone::Update() {

	switch (state_)
	{
	case SquareDangerZone::State::WARNING:
		if (count_++ >= warningCount_) {
			count_ = 0;
			state_ = State::FINALWARNING;
		}
		else {
			float t = count_ / warningCount_;

			float scaleZ = Lerp(t, 0, maxScale_);
			world_.scale_.z = scaleZ;
			world_.translation_.z = scaleZ;
		}
		break;
	case SquareDangerZone::State::FINALWARNING:

		if (count_ >= (finalWarningCount_ / blinkNum_) * currentBlink_) {
			//描画フラグ反転
			if (isDraw_) {
				isDraw_ = false;
			}
			else {
				isDraw_ = true;
			}
		}

		if (count_++ >= finalWarningCount_) {
			isEnd_ = true;
		}
		break;
	case SquareDangerZone::State::Count:
		break;
	default:
		break;
	}

	if (isDraw_) {
		DaiEngine::InstancingObjData data;
		data.worldTransform_ = world_;
		data.color_ = color_;
		data.alpha_ = color_.w;
		object_->SetData(data);
	}
	object_->Update();
}

void SquareDangerZone::Draw() {

	if (isDraw_) {
		object_->Draw();
	}

}