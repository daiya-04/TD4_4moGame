#include "PlayerRoll.h"
#include"Player/Player.h"

#include"Input.h"

PlayerRoll::PlayerRoll()
{
	tree_.name_ = "Roll";
	tree_.SetMonitorValue("nowVelo", &currentVelo_);
	tree_.SetMonitorValue("groundNormal", &player_->parameters_.grandNormal);


	tree_.SetValue("StSpeed", &startSpeed_);
	tree_.SetValue("decelRate", &decelRate);

	tree_.SetValue("slopeGravity", &slopeGravity_);
	tree_.SetValue("flatThreshold", &flatThreshold_);

	tree_.SetValue("cooldownCount", &cooldownCount_);
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
		//currentVelo_=TransformNormal(currentVelo_, player_->GetWorld().matWorld_);
	}

	//初速を与える
	currentVelo_ = currentVelo_.Normalize() * startSpeed_;

	player_->SetAnimationName("PlayerAvoidance");
}

//0-1
float EaseOutCirc(float x) {
	return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
}

// 入力方向を地面に沿った方向へ変形
Vector3 ProjectOnPlane(const Vector3& vec, const Vector3& planeNormal) {
	return vec - planeNormal * Dot(vec, planeNormal);
}

void PlayerRoll::Update()
{
    if (!player_->GetInput()->GetInput(PlayerInput::Roll)) {
        player_->behaviorRequest_ = Player::Behavior::Move;
        player_->parameters_.currentRollCount = cooldownCount_;
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

    Vector3 nextPos = playerPos + moveDir * player_->GetField()->GetBlockWidth();//ブロック幅分進んだ先
    Block* nextBlock = player_->GetField()->GetBlock(nextPos.x, nextPos.z);

    if (currentBlock && nextBlock) {
        float yDiff = nextBlock->world.translation_.y - currentBlock->world.translation_.y;
        float xzDist = (float)std::sqrt(std::pow(nextBlock->world.translation_.x - currentBlock->world.translation_.x, 2) +
            std::pow(nextBlock->world.translation_.z - currentBlock->world.translation_.z, 2));

        if (xzDist > 0.001f) {
            float slopeAngle = std::atan2(yDiff, xzDist); // ラジアン角度

            // 最大角度は45度とし、角度に応じた係数計算
            const float maxSlopeRad = 45.0f * 3.14159f / 180.0f;
            float slopeFactor = slopeAngle / maxSlopeRad;
            slopeFactor = std::clamp(slopeFactor, -1.0f, 1.0f);

            // 上りは減速、下りは加速
            const float uphillDecel = 0.5f;   // 上り最大半減
            const float downhillAccel = 1.2f; // 下り最大1.2倍
            float speedMultiplier = 1.0f;

            if (slopeFactor > 0.0f) {
                speedMultiplier = 1.0f - slopeFactor * (1.0f - uphillDecel);
            }
            else {
                speedMultiplier = 1.0f - slopeFactor * (downhillAccel - 1.0f);
            }

            currentVelo_ *= speedMultiplier;
        }
    }

    // 元の傾斜・減速処理
    Vector3 groundNormal = params.grandNormal;
    Vector3 slopeDir = ProjectOnPlane(Vector3(0, -1, 0), groundNormal);
    slopeDir.y = 0.0f;
    float slopeLen = slopeDir.Length();

    if (slopeLen > flatThreshold_) {
        slopeDir = slopeDir.Normalize();
        Vector3 slopeAccel = slopeDir * slopeGravity_ * slopeLen;
        currentVelo_ += slopeAccel;
    }
    else {
        float spd = currentVelo_.Length();
        if (spd > decelRate) {
            currentVelo_ = currentVelo_.Normalize() * (spd - decelRate);
        }
        else {
            currentVelo_ = Vector3{ 0, 0, 0 };
        }
    }

    // 最終的にプレイヤーに加算
    params.velocity += currentVelo_;

#ifdef _DEBUG
    if (DaiEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        player_->behaviorRequest_ = Player::Behavior::SpinAttack;
    }
#endif // _DEBUG


	if(DaiEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)){
		player_->behaviorRequest_ = Player::Behavior::SpinAttack;
	}


}
