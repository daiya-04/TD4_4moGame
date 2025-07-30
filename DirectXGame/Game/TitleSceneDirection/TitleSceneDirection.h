#pragma once
#include"GameObject.h"
#include"InstancingGameObject/InstancingGameObject.h"
#include <random>

constexpr float PI = 3.14159265f;

class TitleSceneDirection :public GameObject {
public:
	void Initialize();
	void Update()override;
	void Draw()override;
	void Finalize();

	// ターゲット座標へ向かう移動命令
	void MoveTo(const Vector3& target);

private:
	std::unique_ptr<InstancingGameObject> instancingObj_ = nullptr;

	DaiEngine::WorldTransform worldTransform_;

	Vector3 targetPos_;
	Vector3 wanderMin_ = { -5.0f, 0.0f, -5.0f }; // 徘徊範囲
	Vector3 wanderMax_ = { 5.0f, 0.0f, 5.0f };

	float moveSpeed_ = 0.025f;

	bool isGameStart_ = false;

	void UpdateWander();
	Vector3 GetRandomXZPosition();
};