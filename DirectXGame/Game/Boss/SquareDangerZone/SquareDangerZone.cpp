#include "SquareDangerZone.h"

SquareDangerZone::SquareDangerZone()
{
	//オブジェクト生成
	GameObject::Init("Square");

	tree_.name_ = "SquareDangerZone";
	tree_.SetValue("maxScale",&maxScale_);
	tree_.SetValue("warningCount", &warningCount_);
	tree_.SetValue("finalWarningCount", &finalWarningCount_);
}

void SquareDangerZone::Init() {
	isEnd_ = false;
	count_ = 0;
	state_ = State::WARNING;
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
			world_->scale_.z = scaleZ;

		}
		break;
	case SquareDangerZone::State::FINALWARNING:
		if (count_++ >= finalWarningCount_) {
			isEnd_ = true;
		}
		break;
	case SquareDangerZone::State::Count:
		break;
	default:
		break;
	}

	GameObject::Update();
}

void SquareDangerZone::Draw() {

	if (isDraw_) {
		GameObject::Draw();
	}

}