#include "BossUI.h"
#include "TextureManager.h"
#include "Easing.h"

BossGauge::BossGauge() {}

void BossGauge::Init() {

	///セト
	hpGauge_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGauge.png"), {}));
	hpGauge_->SetAnchorpoint({ 1.0f,0.5f });
	gaugeSize_ = hpGauge_->GetSize();

	hpFream_.reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), {}));
	hpFream_->SetAnchorpoint({ 1.0f,0.5f });

}

void BossGauge::Update(const int HP, const int maxHP) {

	stateUpdateTable_[state_]();

	percent_ = static_cast<float>(HP) / static_cast<float>(maxHP);

	curPer_ = Lerp(0.05f, curPer_, percent_);

	hpGauge_->SetSize({ gaugeSize_.x * curPer_, gaugeSize_.y });
	hpGauge_->SetTextureArea({550.0f * (1.0f - curPer_), 0.0f}, {550.0f * curPer_, gaugeSize_.y});
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

void BossGauge::EntendUpdate() {

	param_ += paramSpeed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);

	gaugeSize_.x = Lerp(param_, startSize_, endSize_);
	Vector2 size = hpFream_->GetSize();
	size.x = Lerp(param_, startSize_, endSize_ + 50.0f);

	Vector2 gaugePos = hpGauge_->GetPosition();
	gaugePos.x = Lerp(param_, startPos_, endPos_);

	Vector2 freamPos = hpFream_->GetPosition();
	freamPos.x = Lerp(param_, startPos_, endPos_ + 25.0f);

	hpGauge_->SetPosition(gaugePos);
	
	hpFream_->SetPosition(freamPos);
	hpFream_->SetSize(size);

	if (param_ >= 1.0f) {
		state_ = State::Idle;
	}

}

void BossGauge::StartExtend(const Vector2& size, const Vector2& pos) {

	startSize_ = size.x;
	endSize_ = size.y;

	startPos_ = pos.x;
	endPos_ = pos.y;

	state_ = State::Extend;

	param_ = 0.0f;

}

BossUI::BossUI()
{

	gaugeSetData_.resize(static_cast<size_t>(BossType::Count));
	gaugeSetData_ = {
		{ {630.0f, 60.0f}, {490.0f, 64.0f}, {610.0f, 60.0f}, {450.0f, 64.0f} },
		{ {1145.0f, 60.0f}, {490.0f, 64.0f}, {1125.0f, 60.0f}, {450.0f, 64.0f} },
		{ {0.0f, 60.0f}, {0.0f, 64.0f}, {0.0f, 60.0f}, {0.0f, 64.0f} },
	};

	iconSetData_.resize(static_cast<size_t>(BossType::Count));
	iconSetData_ = {
		{ DaiEngine::TextureManager::Load("bossIcon1.png"), {120.0f, 60.0f} },
		{ DaiEngine::TextureManager::Load("bossIcon2.png"), {640.0f, 60.0f} },
		{ DaiEngine::TextureManager::Load("bossIcon3.png"), {1160.0f, 60.0f} }, 
	};
	
	for (size_t index = 0; index < static_cast<size_t>(BossType::Count); index++) {
		auto& newGauge = gauge_.emplace_back(std::make_unique<BossGauge>());
		newGauge->Init();
		newGauge->SetData(gaugeSetData_[index]);

		auto& newIcon = icons_.emplace_back(DaiEngine::Sprite::Create(iconSetData_[index].texHandle, iconSetData_[index].pos));
		newIcon->SetScale(0.6f);
	}

	gauge_[static_cast<size_t>(BossType::CupCake)]->DrawOff();
}

void BossUI::Update()
{
	
	stateUpdateTable[state_]();

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

void BossUI::FadeOutUpdate() {

	param_ += fadeSpeed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);

	float alpha = Lerp(param_, 1.0f, 0.0f);

	icons_[static_cast<size_t>(BossType::GingerbreadMan)]->SetColor({ 1.0f,1.0f,1.0f,alpha });
	icons_[static_cast<size_t>(BossType::Donut)]->SetColor({ 1.0f,1.0f,1.0f,alpha });

	gauge_[static_cast<size_t>(BossType::GingerbreadMan)]->SetAlpha(alpha);
	gauge_[static_cast<size_t>(BossType::Donut)]->SetAlpha(alpha);

	if (alpha <= 0.0f) {
		StartIconSlide();
	}

}

void BossUI::IconSlideUpdate() {

	slideSpeed_ += slideAccel_;
	param_ += slideSpeed_;
	param_ = std::clamp(param_, 0.0f, 1.0f);

	float T = Easing::easeOutElastic(param_);

	Vector2 pos = Lerp(T, startPos_, endPos_);


	icons_[static_cast<size_t>(BossType::CupCake)]->SetPosition(pos);


	if (param_ >= 1.0f) {
		state_ = State::Idle;
		gauge_[static_cast<size_t>(BossType::CupCake)]->StartExtend({0.0f,550.0f}, {endPos_.x + 40.0f, endPos_.x + 40.0f + 550.0f});
	}

}

void BossUI::SetBossData(const std::vector<std::unique_ptr<IBoss>>& bosses) {

	bosses_.clear();

	for (auto& boss : bosses) {
		bosses_.push_back(boss.get());
	}

}

void BossUI::StartFadeOut() {

	state_ = State::FadeOut;

	param_ = 0.0f;

}

void BossUI::StartIconSlide() {

	icons_[static_cast<size_t>(BossType::GingerbreadMan)]->DrawOff();
	icons_[static_cast<size_t>(BossType::Donut)]->DrawOff();

	gauge_[static_cast<size_t>(BossType::GingerbreadMan)]->DrawOff();
	gauge_[static_cast<size_t>(BossType::Donut)]->DrawOff();

	gauge_[static_cast<size_t>(BossType::CupCake)]->DrawOn();

	state_ = State::IconSlide;

	startPos_ = icons_[static_cast<size_t>(BossType::CupCake)]->GetPosition();
	endPos_ = icons_[static_cast<size_t>(BossType::GingerbreadMan)]->GetPosition();
	endPos_.x = 400.0f;

	param_ = 0.0f;

}