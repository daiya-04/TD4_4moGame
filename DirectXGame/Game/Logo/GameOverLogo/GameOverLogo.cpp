#include "GameOverLogo.h"

#include "TextureManager.h"


void GameOverLogo::Init(const std::wstring& text) {

	uint32_t texHandle = DaiEngine::TextureManager::Load("gameOver.png");

	for (size_t index = 0; index < text.size(); index++) {
		auto& newChar = text_.emplace_back(std::make_unique<GameOverChar>());
		newChar->Init(texHandle, index);
		newChar->SetPos({ 640.0, -200.0f });
	}


	stateRequest_ = AnimeState::Apper;

}

void GameOverLogo::Update() {

	if (stateRequest_) {

		state_ = stateRequest_.value();

		stateInitTable_[state_]();

		stateRequest_ = std::nullopt;
	}

	stateUpdateTable_[state_]();

	for (auto& ch : text_) {
		ch->Update();
	}

}

void GameOverLogo::Draw() {
	for (auto& ch : text_) {
		ch->Draw();
	}
}

void GameOverLogo::ApperInit() {

	timer_ = 0.0f;

}

void GameOverLogo::ApperUpdate() {

	timer_ += kDeltaTime_;

	float charWidth = 256.0f;

	if (apperedIndex_ < text_.size()) {
		if (timer_ >= apperInterval_) {
			int32_t offset = static_cast<int32_t>(apperedIndex_ - (text_.size() / 2));
			text_[apperedIndex_++]->StartApper(
				{ 640.0f + offset * (charWidth - (charWidth / 2.0f)) , -200.0f },
				{ 640.0f + offset * (charWidth - (charWidth / 2.0f)) , 200.0f }
			);

			timer_ = 0.0f;
		}
	}
	else {
		if (text_[text_.size() - 1]->FinishApper()) {
			stateRequest_ = AnimeState::Floating;
		}
	}

}

void GameOverLogo::FloatingInit() {

	for (auto& ch : text_) {
		ch->StartFloat();
	}

}

void GameOverLogo::FloatingUpdate() {



}
