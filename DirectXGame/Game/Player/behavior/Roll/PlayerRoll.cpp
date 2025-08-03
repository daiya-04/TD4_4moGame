#include "PlayerRoll.h"
#include"Player/Player.h"

#include "EffectManager.h"

#include"Input.h"

PlayerRoll::PlayerRoll()
{
	tree_.name_ = "Roll";
	tree_.SetMonitorValue("nowVelo", &currentVelo_);
	tree_.SetMonitorValue("groundNormal", &player_->parameters_.grandNormal);
    tree_.SetMonitorValue("chargeCount", &chargeJump_);

	tree_.SetValue("StSpeed", &startSpeed_);
	tree_.SetValue("decelRate", &decelRate);

	tree_.SetValue("slopeGravity", &slopeGravity_);
	tree_.SetValue("flatThreshold", &flatThreshold_);

	tree_.SetValue("cooldownCount", &cooldownCount_);

	tree_.SetValue("uphillDecel", &uphillDecel_);
	tree_.SetValue("downhillAccel", &downhillAccel_);

    tree_.SetValue("turnInfluence", &turnInfluence_);

    tree_.SetValue("maxCharge", &maxCharge_);
    tree_.SetValue("maxRollSpeed", &maxRollSpeed_);

    player_->GetUI()->SetCharge(&chargeJump_, maxCharge_);

}

void PlayerRoll::Init()
{
	//入力方向チェック
	currentVelo_ = player_->SetBody2Input();

	//もし入力がない場合
	if (currentVelo_ == Vector3{0, 0, 0}) {
		//前に進む
		currentVelo_ = {0,0,1};
		
		//プレイヤー方向に向ける
		currentVelo_=TransformNormal(currentVelo_, player_->GetWorld().matWorld_);
	}

	//初速を与える
	currentVelo_ = currentVelo_.Normalize() * startSpeed_;

    //チャージの初期化
    chargeJump_ = 0.0f;

	player_->SetAnimationName("PlayerAvoidance");

    emitPos_ = player_->GetWorld().GetWorldPos();
    EffectManager::GetInstance()->Start("PlayerMoveEffect", &emitPos_);

    lastPos_ = player_->GetWorld().GetWorldPos();
}

//0-1
float EaseOutCirc(float x) {
	return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
}

// 入力方向を地面に沿った方向へ変形
Vector3 ProjectOnPlane(const Vector3& vec, const Vector3& planeNormal) {
	return vec - planeNormal * Dot(vec, planeNormal);
}

void PlayerRoll::Update() {

    emitPos_ = player_->GetWorld().GetWorldPos();

    if (!player_->GetInput()->GetInput(PlayerInput::Roll)) {
        player_->behaviorRequest_ = Player::Behavior::Move;
        player_->parameters_.currentRollCount = cooldownCount_;
        EffectManager::GetInstance()->End("PlayerMoveEffect");
        player_->SetJumpAttackFlag(false);
        return;
    }

    PlayerParameters& params = player_->parameters_;

    // 移動方向（XZのみ）
    Vector3 moveDir = currentVelo_;
    moveDir.y = 0.0f;
    if (moveDir.Length() < 0.001f) {
        moveDir = Vector3(0, 0, 1); // デフォルト前方向
    }
    moveDir = moveDir.Normalize();

    // 現在位置・移動先のブロック取得
    Vector3 playerPos = player_->GetWorld().translation_;
    Block* currentBlock = player_->GetField()->GetBlock(playerPos.x, playerPos.z);

    Vector3 nextPos = playerPos + moveDir * player_->GetField()->GetBlockWidth();
    Block* nextBlock = player_->GetField()->GetBlock(nextPos.x, nextPos.z);

    bool isSliding = false;

    if (currentBlock && nextBlock) {
        float yDiff = nextBlock->world.translation_.y - currentBlock->world.translation_.y;
        float xzDist = (float)std::sqrt(
            std::pow(nextBlock->world.translation_.x - currentBlock->world.translation_.x, 2) +
            std::pow(nextBlock->world.translation_.z - currentBlock->world.translation_.z, 2));

        if (xzDist > 0.001f) {
            float slopeAngle = std::atan2(yDiff, xzDist);

            // 角度によって加速・減速
            const float maxSlopeRad = 45.0f * 3.14159f / 180.0f;
            float slopeFactor = slopeAngle / maxSlopeRad;
            slopeFactor = std::clamp(slopeFactor, -1.0f, 1.0f);

            float speedMultiplier = 1.0f;
            if (slopeFactor > 0.0f) {
                speedMultiplier = 1.0f - slopeFactor * (1.0f - uphillDecel_);
            }
            else {
                speedMultiplier = 1.0f - slopeFactor * (downhillAccel_ - 1.0f);
            }

            currentVelo_ *= speedMultiplier;

            // 滑り判定：一定以上の上り下り坂
            const float slideThreshold = 0.0001f * 3.14159f / 180.0f;
            if (std::fabs(slopeAngle) > slideThreshold && std::fabs(yDiff) > 0.01f) {
                isSliding = true;
            }
        }
    }

    // 減速処理（傾斜無し）
    float spd = currentVelo_.Length();
    if (spd > decelRate) {
        currentVelo_ = currentVelo_.Normalize() * (spd - decelRate);
    }
    else {
        currentVelo_ = Vector3{ 0, 0, 0 };
    }

    // 滑り中にチャージ
    Vector3 currentPos = player_->GetWorld().GetWorldPos();
    float movedDistance = (currentPos - lastPos_).Length();

    // 滑っており、かつ移動している場合のみチャージ
    if (isSliding && movedDistance > 0.01f) {
        chargeJump_ += 1.0f;
        if (chargeJump_ > maxCharge_) {
            chargeJump_ = maxCharge_;
            player_->SetJumpAttackFlag(true);
        }
    }
    else {
        if (chargeJump_ < 0.0f) {
            chargeJump_ = 0.0f;
        }
        player_->SetJumpAttackFlag(false);
    }

    // 最後に位置を更新
    lastPos_ = currentPos;

    player_->GetUI()->SetCharge(&chargeJump_, maxCharge_);

    // 入力取得（方向制御に使う）
    Vector3 move = player_->SetBody2Input();

    // ロール中、入力に応じて方向を少し曲げる
    if (move.Length() > 0.001f) {
        move = move.Normalize();
        float speed = currentVelo_.Length();
        Vector3 blendedDir = (currentVelo_.Normalize() * (1.0f - turnInfluence_) + move * turnInfluence_).Normalize();
        currentVelo_ = blendedDir * speed;
    }

    if (currentVelo_.Length() > maxRollSpeed_) {
        currentVelo_ = currentVelo_.Normalize() * maxRollSpeed_;
    }

    params.velocity = currentVelo_;

    // チャージ完了時スピン
    if ((DaiEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)|| DaiEngine::Input::GetInstance()->TriggerButton(DaiEngine::Input::Button::A)) && chargeJump_ >= maxCharge_) {
        player_->behaviorRequest_ = Player::Behavior::SpinAttack;
        EffectManager::GetInstance()->End("PlayerMoveEffect");
        chargeJump_ = 0.0f;
        player_->SetJumpAttackFlag(false);
    }
}
