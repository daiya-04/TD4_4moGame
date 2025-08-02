#include "GameOverChar.h"

#include "Easing.h"
#include "RandomEngine.h"

void GameOverChar::Init(uint32_t textureHandle, size_t charNum) {

	BaseChar::Init(textureHandle, charNum);

	state_ = AnimeState::Idle;

	ch_->SetTextureArea({ 128.0f * charNum_, 0.0f }, { 128.0f,128.0f });
	ch_->SetSize(size_);

}

void GameOverChar::Update() {

	stateUpdateTable_[state_]();

	BaseChar::Update();

}

void GameOverChar::ApperUpdate() {

	param_ += paramSpeed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);
	float T = Easing::easeOutBounce(param_);

	pos_ = Lerp(T, startPos_, endPos_);

	if (param_ >= 1.0f) {
		finishApper_ = true;
	}

}

void GameOverChar::FloatingUpdate() {

	timer_ += kDeltaTime_ * 2.0f; 

	Vector2 randomOffset = Vector2(
		std::sin(seed_.x + timer_ * 1.0f),
		std::cos(seed_.y + timer_ * 1.0f + 1.0f)
	);

	float strength = 5.0f;

	pos_ = BasePos_ + (randomOffset * strength);

}

void GameOverChar::IdleUpdate() {



}

void GameOverChar::StartApper(const Vector2& startPos, const Vector2& targetPos) {

	startPos_ = startPos;
	endPos_ = targetPos;
	state_ = AnimeState::Apper;

}

void GameOverChar::StartFloat() {

	state_ = AnimeState::Floating;
	seed_ = { RandomEngine::GetFloatRandom(0.0f, 5.0f), RandomEngine::GetFloatRandom(0.0f, 5.0f) };
	BasePos_ = pos_;

}

