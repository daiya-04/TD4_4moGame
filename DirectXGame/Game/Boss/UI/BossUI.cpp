#include "BossUI.h"
#include "TextureManager.h"

BossUI::BossUI()
{
	///セト
	hpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGauge.png"), {}));
	hpGauge_->SetAnchorpoint({ 0.0f,0.5f });
	hpGauge_->SetPosition({ 365.0f,60.0f });
	gaugeSize_ = hpGauge_->GetSize();

	hpFream_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), { 340.0f,60.0f }));
	hpFream_->SetAnchorpoint({ 0.0f,0.5f });

	icon_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("bossIcon1.png"), { 330.0f,60.0f }));
	icon_->SetScale(0.6f);
}

void BossUI::Update(const int HP, const int maxHP)
{
	percent_ = static_cast<float>(HP) / static_cast<float>(maxHP);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({}, { gaugeSize_.x * curPer_, gaugeSize_.y });
}

void BossUI::Draw()
{
	hpFream_->Draw();
	hpGauge_->Draw();
	icon_->Draw();
}