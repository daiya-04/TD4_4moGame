#pragma once
#include "Sprite.h"

#include <memory>
#include <map>
#include <functional>

class CrossMark {
public:

	void Init(const Vector2& pos);

	void Update();

	void Draw();

	void StartStamp(float speed, float startScale);

private:

	std::unique_ptr<DaiEngine::Sprite> ui_;

private:

	enum State {
		Idle,
		Stamp,
	};

	State state_ = State::Idle;

	std::map<State, std::function<void()>> stateUpdateTable_ = {
		{State::Idle, [this]() {IdleUpdate(); }},
		{State::Stamp, [this]() {StampUpdate(); }},
	};

	float param_ = 0.0f;
	float speed_ = 0.0f;

	float StartScale_ = 0.0f;
	float endScale_ = 0.0f;

private:

	void IdleUpdate(){}
	void StampUpdate();

};

