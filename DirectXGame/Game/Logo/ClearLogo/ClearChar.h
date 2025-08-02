#pragma once
#include "BaseChar.h"
#include <map>
#include <functional>
#include <optional>



class ClearChar : public BaseChar {
public:

	void Init(uint32_t textureHandle, size_t charNum) override;

	void Update() override;

	void StartApper(const Vector2& targetPos);
	void SetParam(float param) { param_ = param; }

	void StartBounce();

	void SetInitVelo(float initVelo) { initVelo_ = initVelo; }
	void SetGravity(float gravity) { gravity_ = gravity; }

private:

	Vector2 size_ = { 128.0f,128.0f };
	float scale_ = 0.0f;
	float rotate_ = 0.0f;

	Vector2 startPos_{};
	Vector2 endPos_{};
	float param_ = 0.0f;

	float velocity_ = 0.0f;
	float initVelo_ = -10.0f;
	float gravity_ = 0.5f;

private:

	enum class AnimeState {
		Apper,
		Bounce,
		Idle,
	};

	AnimeState state_ = AnimeState::Idle;
	std::optional<AnimeState> stateRequest_ = std::nullopt;

	std::map<AnimeState, std::function<void()>> stateUpdateTable_ = {
		{AnimeState::Apper, [this]() {ApperUpdate(); }},
		{AnimeState::Bounce, [this]() { BounceUpdate(); }},
		{AnimeState::Idle, [this]() {IdleUpdate(); }},
	};

private:

	void IdleUpdate();

	void ApperUpdate();

	void BounceUpdate();

};

