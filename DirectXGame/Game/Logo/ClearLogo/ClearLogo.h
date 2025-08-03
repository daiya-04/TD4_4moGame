#pragma once
#include "ClearChar.h"

#include <vector>
#include <string>
#include <memory>


class ClearLogo {
public:

	void Init(const std::wstring& text);

	void Update();

	void Draw();

	void StartApper() { stateRequest_ = AnimeState::Apper; }
	void StartBounce() { stateRequest_ = AnimeState::Bounce; }

	bool IsAppered() const { return param_ >= 1.0f; }

private:

	std::vector<std::unique_ptr<ClearChar>> text_;

private:

	enum class AnimeState {
		Apper,
		Bounce,
		Idle,
	};

	AnimeState state_ = AnimeState::Idle;
	std::optional<AnimeState> stateRequest_ = std::nullopt;

	std::map<AnimeState, std::function<void()>> stateInitTable_ = {
		{AnimeState::Apper, [this]() {ApperInit(); }},
		{AnimeState::Bounce, [this]() { BounceInit(); }},
		{AnimeState::Idle, [this]() { IdleInit(); }},
	};

	std::map<AnimeState, std::function<void()>> stateUpdateTable_ = {
		{AnimeState::Apper, [this]() {ApperUpdate(); }},
		{AnimeState::Bounce, [this]() { BounceUpdate(); }},
		{AnimeState::Idle, [this]() { IdleUpdate(); }},
	};


	float param_ = 0.0f;
	float paramSpeed_ = 1.0f / 120.0f;

	float timer_ = 0.0f;
	const float kDeltaTime_ = 1.0f / 60.0f;

	size_t bounceIndex_ = 0;
	float bounceInterval_ = 0.25f;
	float reStartInterval_ = 2.0f;

private:

	void ApperInit();
	void ApperUpdate();

	void BounceInit();
	void BounceUpdate();

	void IdleInit() {}
	void IdleUpdate() {}


};

