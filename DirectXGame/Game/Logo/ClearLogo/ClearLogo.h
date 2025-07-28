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

private:

	std::vector<std::unique_ptr<ClearChar>> text_;

private:

	AnimeState state_ = AnimeState::Apper;
	std::optional<AnimeState> stateRequest_ = std::nullopt;

	std::map<AnimeState, std::function<void()>> stateInitTable_ = {
		{AnimeState::Apper, [this]() {ApperInit(); }},
		{AnimeState::Bounce, [this]() { BounceInit(); }},
	};

	std::map<AnimeState, std::function<void()>> stateUpdateTable_ = {
		{AnimeState::Apper, [this]() {ApperUpdate(); }},
		{AnimeState::Bounce, [this]() { BounceUpdate(); }},
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


};

