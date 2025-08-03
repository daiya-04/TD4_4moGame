#include "CrossMark.h"

#include "TextureManager.h"
#include "Easing.h"

void CrossMark::Init(const Vector2& pos) {

	ui_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("batu.png"), pos));
	ui_->SetScale(0.0f);

}

void CrossMark::Update() {

	stateUpdateTable_[state_]();

}

void CrossMark::Draw() {
	ui_->Draw();
}

void CrossMark::StampUpdate() {

	param_ += speed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);

	float T = Easing::easeInExpo(param_);

	float rotate = Lerp(param_, 0.0f, 3.14f * 8.0f);
	ui_->SetRotate(rotate);

	float scale = Lerp(T, StartScale_, endScale_);
	ui_->SetScale(scale);

}

void CrossMark::StartStamp(float speed, float startScale) {

	speed_ = speed;
	StartScale_ = startScale;
	endScale_ = 1.0f;

	state_ = State::Stamp;

}
