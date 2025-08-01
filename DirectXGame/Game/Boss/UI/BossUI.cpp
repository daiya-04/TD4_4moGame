#include "BossUI.h"
#include "TextureManager.h"


BossGauge::BossGauge() {}

void BossGauge::Init() {

	///セト
	hpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGauge.png"), {}));
	hpGauge_->SetAnchorpoint({ 0.0f,0.5f });
	hpGauge_->SetPosition({ 365.0f,60.0f });
	gaugeSize_ = hpGauge_->GetSize();

	hpFream_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), { 340.0f,60.0f }));
	hpFream_->SetAnchorpoint({ 0.0f,0.5f });

}

void BossGauge::Update(const int HP, const int maxHP) {
	percent_ = static_cast<float>(HP) / static_cast<float>(maxHP);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({}, { gaugeSize_.x * curPer_, gaugeSize_.y });
}

void BossGauge::Draw() {

	hpFream_->Draw();
	hpGauge_->Draw();

}

void BossGauge::SetData(const GaugeSetData& data) {

	hpGauge_->SetPosition(data.gaugePos);
	hpGauge_->SetSize(data.gaugeSize);
	gaugeSize_ = data.gaugeSize;

	hpFream_->SetPosition(data.freamPos);
	hpFream_->SetSize(data.freamSize);

}

BossUI::BossUI()
{

	gaugeSetData_.resize(static_cast<size_t>(BossType::Count));
	gaugeSetData_ = {
		{ {}, {}, {}, {} },
		{ {}, {}, {}, {} },
		{ {}, {0.0f, 64.0f}, {}, {0.0f, 64.0f} },
	};

	iconSetData_.resize(static_cast<size_t>(BossType::Count));
	iconSetData_ = {
		{ DaiEngine::TextureManager::Load("bossIcon1.png"), {} },
		{ DaiEngine::TextureManager::Load("bossIcon2.png"), {} },
		{ DaiEngine::TextureManager::Load("bossIcon3.png"), {} }, 
	};
	
	for (size_t index = 0; index < static_cast<size_t>(BossType::Count); index++) {
		auto& newGauge = gauge_.emplace_back(std::make_unique<BossGauge>());
		newGauge->Init();
		newGauge->SetData(gaugeSetData_[index]);

		auto& newIcon = icons_.emplace_back(DaiEngine::Sprite::Create(iconSetData_[index].texHandle, iconSetData_[index].pos));
		newIcon->SetScale(0.6f);
	}
}

void BossUI::Update()
{
	


	for (size_t index = 0; index < static_cast<size_t>(BossType::Count); index++) {
		gauge_[index]->Update(bosses_[index]->GetHP(), bosses_[index]->GetMaxHP());
	}
}

void BossUI::Draw()
{

	for (size_t index = 0; index < static_cast<size_t>(BossType::Count); index++) {
		gauge_[index]->Draw();
		icons_[index]->Draw();
	}
}

void BossUI::SetBossData(const std::vector<std::unique_ptr<IBoss>>& bosses) {

	bosses_.clear();

	for (auto& boss : bosses) {
		bosses_.push_back(boss.get());
	}

}