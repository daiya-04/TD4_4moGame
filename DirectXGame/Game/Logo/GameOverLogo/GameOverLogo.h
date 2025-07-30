#pragma once
#include "GameOverChar.h"

#include <vector>
#include <string>
#include <memory>

class GameOverLogo {
public:

	void Init(const std::wstring& text);

	void Update();

	void Draw();

private:

	std::vector<std::unique_ptr<GameOverChar>> text_;

private:

	enum class AnimeState {
		Apper,
		Floating,
	};

	AnimeState state_ = AnimeState::Apper;
	std::optional<AnimeState> stateRequest_ = std::nullopt;

	std::map<AnimeState, std::function<void()>> stateInitTable_ = {
		{AnimeState::Apper, [this]() {ApperInit(); }},
		{AnimeState::Floating, [this]() {FloatingInit(); }},
	};

	std::map<AnimeState, std::function<void()>> stateUpdateTable_ = {
		{AnimeState::Apper, [this]() {ApperUpdate(); }},
		{AnimeState::Floating, [this]() {FloatingUpdate(); }},
	};

	float timer_ = 0.0f;
	const float kDeltaTime_ = 1.0f / 60.0f;
	float apperInterval_ = 0.25f;

	size_t apperedIndex_ = 0;

private:

	void ApperInit();
	void ApperUpdate();

	void FloatingInit();
	void FloatingUpdate();

};

