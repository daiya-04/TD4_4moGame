#pragma once
#include "BaseChar.h"

#include <map>
#include <functional>
#include <optional>



class GameOverChar : public BaseChar {
public:

	void Init(uint32_t textureHandle, size_t charNum) override;

	void Update();

	void StartApper(const Vector2& startPos, const Vector2& targetPos);
	void SetParam(float param) { param_ = param; }

	void StartFloat();

	bool FinishApper() const { return finishApper_; }

private:

	Vector2 size_ = { 128.0f,128.0f };
	
	Vector2 startPos_{};
	Vector2 endPos_{};

	float param_ = 0.0f;
	float paramSpeed_ = 1.0f / 180.0f;

	float finishApper_ = false;

	float timer_ = 0.0f;
	const float kDeltaTime_ = 1.0f / 60.0f;
	Vector2 seed_{};
	Vector2 BasePos_{};

private:

	enum class AnimeState {
	Apper,
	Floating,
	Idle,
	};

	AnimeState state_ = AnimeState::Idle;
	std::optional<AnimeState> stateRequest_ = std::nullopt;

	std::map<AnimeState, std::function<void()>> stateUpdateTable_ = {
		{AnimeState::Apper, [this]() {ApperUpdate(); }},
		{AnimeState::Floating, [this]() {FloatingUpdate(); }},
		{AnimeState::Idle, [this]() {IdleUpdate(); }},
	};


private:

	void IdleUpdate();

	void ApperUpdate();

	void FloatingUpdate();

};

