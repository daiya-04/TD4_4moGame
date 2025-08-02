#include "TitleSceneDirection.h"
#include <cmath>

void TitleSceneDirection::Initialize() {
	//Instancingゲームオブジェクト
	instancingObj_ = std::make_unique<InstancingGameObject>();
	instancingObj_->Init("StartField", 10000);

	worldTransform_.Init();
	worldTransform_.scale_ *= 10.0f;
    worldTransform_.scale_.x *= 5.0f;
	worldTransform_.UpdateMatrix();

	targetPos_ = GetRandomXZPosition(); // 初期目標

	GameObject::Init("PlayerIdle");
	SetAnimationName("PlayerWalk");

	world_->translation_.y = 1.5f;
}

void TitleSceneDirection::Update() {
	UpdateWander();

	// インスタンシング用データ登録
	DaiEngine::InstancingObjData data;
	data.worldTransform_ = worldTransform_;
	instancingObj_->SetData(data);

	GameObject::Update();
}

void TitleSceneDirection::Draw() {
	instancingObj_->Draw();
	GameObject::Draw();
}

void TitleSceneDirection::Finalize() {
}

void TitleSceneDirection::MoveTo(const Vector3& target) {
	targetPos_ = target;
    isGameStart_ = true;
}

void TitleSceneDirection::UpdateWander() {
    Vector3 pos = world_->translation_;
    Vector3 toTarget = targetPos_ - pos;
    toTarget.y = 0.0f;

    float distance = toTarget.Length();
    if (distance < 0.5f) {
        if (isGameStart_ == false) {
            // 到達 → 新しい目標設定
            targetPos_ = GetRandomXZPosition();
        }
        else {//ゲーム開始時に消える
            SetAnimationName("PlayerDown");
        }
    }
    else {
        // 目標への方向
        Vector3 dir = toTarget.Normalize();

        // 現在の向きと目標角度
        float currentYaw = world_->rotation_.y;
        float targetYaw = std::atan2(dir.x, dir.z);
        float turnSpeed = 0.05f;

        // -π〜πの範囲に補正
        float deltaYaw = targetYaw - currentYaw;
        if (deltaYaw > PI) deltaYaw -= PI * 2.0f;
        if (deltaYaw < -PI) deltaYaw += PI * 2.0f;

        // 少しずつ回転
        world_->rotation_.y += deltaYaw * turnSpeed;

        // 現在の向きに応じた前方ベクトル
        Vector3 forward = {
            std::sin(world_->rotation_.y),
            0.0f,
            std::cos(world_->rotation_.y)
        };

        // 進行
        pos += forward * moveSpeed_;
        world_->translation_ = pos;
    }

}

Vector3 TitleSceneDirection::GetRandomXZPosition() {
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> distX(wanderMin_.x, wanderMax_.x);
	std::uniform_real_distribution<float> distZ(wanderMin_.z, wanderMax_.z);

	return { distX(rng), 1.5f, distZ(rng) };
}
