#include "ClearLogo.h"
#include "TextureManager.h"
#include "Easing.h"
#include "ImGuiManager.h"


void ClearLogo::Init(const std::wstring& text) {

	uint32_t texHandle = DaiEngine::TextureManager::Load("gameClear.png");

	for (size_t index = 0; index < text.size(); index++) {
		//追加
		auto& newChar = text_.emplace_back(std::make_unique<ClearChar>());
		newChar->Init(texHandle, index);
		newChar->SetPos({ 640.0f, 400.0f });
	}

	stateRequest_ = AnimeState::Idle;

}

void ClearLogo::Update() {

#ifdef _DEBUG

	ImGui::Begin("ClearLogo");

	ImGui::SliderFloat("BounceInterval", &bounceInterval_, 0.0f, 2.0f);
	ImGui::SliderFloat("ReStartInterval_", &reStartInterval_, 0.0f, 5.0f);

	ImGui::End();

#endif // _DEBUG

	if (stateRequest_) {

		state_ = stateRequest_.value();

		stateInitTable_[state_]();

		stateRequest_ = std::nullopt;
	}

	stateUpdateTable_[state_]();

	//各文字の更新
	for (auto& ch : text_) {
		ch->Update();
	}

}

void ClearLogo::Draw() {
	for (auto& ch : text_) {
		ch->Draw();
	}
}

void ClearLogo::ApperInit() {

	float charWidth = 128.0f;

	for (size_t index = 0; index < text_.size(); index++) {
		int32_t offset = static_cast<int32_t>(index - ((text_.size() - 1) / 2));
		//演出開始
		text_[index]->StartApper({ 640.0f + offset * charWidth - (charWidth / 2.0f), 200.0f });

	}

}

void ClearLogo::ApperUpdate() {

	param_ += paramSpeed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);
	float T = Easing::easeOutExpo(param_);

	for (auto& ch : text_) {
		ch->SetParam(T);
	}

}

void ClearLogo::BounceInit() {

	timer_ = 0.0f;

}

void ClearLogo::BounceUpdate() {

	timer_ += kDeltaTime_;

	if (bounceIndex_ < text_.size()) {

		if (timer_ >= bounceInterval_) {
			text_[bounceIndex_++]->StartBounce();
			timer_ = 0.0f;
		}

	}
	else {
		if (timer_ >= reStartInterval_) {
			timer_ = 0.0f;
			bounceIndex_ = 0;
		}
	}

}
