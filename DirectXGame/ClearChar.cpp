#include "ClearChar.h"
#include "ImGuiManager.h"


void ClearChar::Init(uint32_t textureHandle, size_t charNum) {

	BaseChar::Init(textureHandle, charNum);

	scale_ = 0.0f;
	state_ = AnimeState::Apper;
	
	ch_->SetTextureArea({ 128.0f * charNum_, 0.0f }, { 128.0f,128.0f });
	ch_->SetSize(size_);
	ch_->SetScale(scale_);

}

void ClearChar::Update() {

	stateUpdateTable_[state_]();
	
	BaseChar::Update();
}

void ClearChar::ApperUpdate() {

	pos_ = Lerp(param_, startPos_, endPos_);
	scale_ = Lerp(param_, 0.0f, 1.0f);
	rotate_ = Lerp(param_, 0.0f, 3.14f * 2.0f);
	ch_->SetScale(scale_);
	ch_->SetRotate(rotate_);
}

void ClearChar::BounceUpdate() {

	velocity_ += gravity_;
	pos_.y += velocity_;

	if (pos_.y >= endPos_.y) {
		pos_.y = endPos_.y;
		state_ = AnimeState::Idel;
	}

}

void ClearChar::IdelUpdate() {



}

void ClearChar::StartApper(const Vector2& targetPos) {

	startPos_ = pos_;
	endPos_ = targetPos;
	state_ = AnimeState::Apper;

}

void ClearChar::StartBounce() {

	state_ = AnimeState::Bounce;
	velocity_ = initVelo_;

}
