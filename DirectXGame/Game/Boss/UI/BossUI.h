#pragma once
#include "Sprite.h"
#include "Boss/BossType/BossType.h"
#include"../IBoss/IBoss.h"

#include <vector>
#include<memory>
#include <optional>
#include <functional>
#include <map>


struct GaugeSetData {
	Vector2 freamPos{};
	Vector2 freamSize{};
	Vector2 gaugePos{};
	Vector2 gaugeSize{};
};

class BossGauge {
public:

	BossGauge();
	~BossGauge() = default;

	void Init();

	void Update(const int HP_, const int maxHP);

	void Draw();

	void SetData(const GaugeSetData& data);

	void StartExtend(const Vector2& size, const Vector2& pos);

	void DrawOn() { hpFream_->DrawOn(); hpGauge_->DrawOn(); }

	void DrawOff() { hpFream_->DrawOff(); hpGauge_->DrawOff(); }

	void SetAlpha(float alpha) { hpGauge_->SetColor({ 1.0f,1.0f,1.0f,alpha }); hpFream_->SetColor({ 1.0f,1.0f,1.0f,alpha }); }

private:

	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;

	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

private:

	enum class State {
		Idle,
		Extend,
	};

	State state_ = State::Idle;

	std::map<State, std::function<void()>> stateUpdateTable_ = {
		{State::Idle, [this]() { IdleUpdate(); }},
		{State::Extend, [this]() {EntendUpdate(); }},
	};

	float startSize_{};
	float endSize_{};

	float startPos_{};
	float endPos_{};

	float param_ = 0.0f;
	float paramSpeed_ = 0.02f;

public:

	void IdleUpdate() {}
	void EntendUpdate();

};

class BossUI {
private:

	struct IconSetData {
		uint32_t texHandle = 0;
		Vector2 pos{};
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossUI();
	~BossUI()=default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="HP_"></param>
	/// <param name="maxHP"></param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetBossData(const std::vector<std::unique_ptr<IBoss>>& bosses);

	void StartFadeOut();
	void StartIconSlide();

private:

	std::vector<IBoss*> bosses_;
	std::vector<std::unique_ptr<BossGauge>> gauge_;
	std::vector<GaugeSetData> gaugeSetData_;

	//UI
	std::vector<std::unique_ptr<DaiEngine::Sprite>> icons_;
	std::vector<IconSetData> iconSetData_;

private:

	enum class State {
		Idle,
		FadeOut,
		IconSlide,
	};

	State state_ = State::Idle;

	std::map<State, std::function<void()>> stateUpdateTable = {
		{State::Idle, [this]() {IdleUpdate(); }},
		{State::FadeOut, [this]() {FadeOutUpdate(); }},
		{State::IconSlide, [this]() {IconSlideUpdate(); }},
	};

	float param_ = 0.0f;
	float fadeSpeed_ = 1.0f / 30.0f;

	float slideSpeed_ = 0.0f;
	float slideAccel_ = 0.001f;

	Vector2 startPos_{};
	Vector2 endPos_{};

private:

	void IdleUpdate() {}
	void FadeOutUpdate();
	void IconSlideUpdate();

};