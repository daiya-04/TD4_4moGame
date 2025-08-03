#include "PlayerUI.h"
#include"TextureManager.h"

PlayerUI::PlayerUI()
{
	///セト
	hpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("playerHPGage.png"), {}));
	hpGauge_->SetAnchorpoint({ 0.0f,0.5f });
	hpGauge_->SetPosition({ 757.0f,670.0f });
	gaugeSize_ = hpGauge_->GetSize();

	hpFream_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("playerHPGageFram.png"), { 700.0f,670.0f }));
	hpFream_->SetAnchorpoint({ 0.0f,0.5f });

	jumpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("speedGauge.png"), {1150.0, 600.0f}));
	jumpGauge_->SetSize({ 200.0f, 200.0f });
	jumpGauge_->SetTextureArea({}, { 200.0f, 200.0f });

}

void PlayerUI::Update()
{
	percent_ = static_cast<float>(*hp_) / static_cast<float>(maxHp_);
	percent_ = std::clamp(percent_, 0.0f, 1.0f);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({}, { gaugeSize_.x * curPer_, gaugeSize_.y });

	percent_ = *jumpCharge_ / maxCharge_;
	percent_ = std::clamp(percent_, 0.0f, 1.0f);

	int index = static_cast<uint8_t>(percent_ * 5.0f);

	jumpGauge_->SetTextureArea({200.0f * index, 0.0f}, { 200.0f, 200.0f });
}

void PlayerUI::DrawUI()
{
	hpFream_->Draw();
	hpGauge_->Draw();

	if (*isSlide_) {
		jumpGauge_->Draw();
	}

}