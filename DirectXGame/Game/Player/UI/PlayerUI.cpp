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
}

void PlayerUI::Update(int currentHP, int maxHP)
{
	percent_ = static_cast<float>(currentHP) / static_cast<float>(maxHP);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({}, { gaugeSize_.x * curPer_, gaugeSize_.y });
}

void PlayerUI::DrawUI()
{
	hpFream_->Draw();
	hpGauge_->Draw();
}