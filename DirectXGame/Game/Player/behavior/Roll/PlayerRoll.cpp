#include "PlayerRoll.h"
#include"Player/Player.h"


PlayerRoll::PlayerRoll()
{
	tree_.name_ = "Roll";
	tree_.SetMonitorValue("nowVelo", &currentVelo_);


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
	//押されていなければ終了
	if (!player_->GetInput()->GetInput(PlayerInput::Roll)) {
		//通常状態にリクエスト
		player_->behaviorRequest_ = Player::Behavior::Move;
		//回避クールタイムを再セット
		player_->parameters_.currentRollCount = cooldownCount_;

		//処理を終える
		return;
	}

	//パラメータ取得
	PlayerParameters params = player_->parameters_;

	//地面の傾斜から加速度方向を取得
	Vector3 groundNormal = params.grandNormal;

	//傾斜方向を求める
	Vector3 slopeDir = ProjectOnPlane(Vector3(0, -1, 0), groundNormal);
	slopeDir.y = 0.0f;
	float slopeLen = slopeDir.Length();

	//傾きがある場合
	if (slopeLen > flatThreshold_) {
		// 傾きがある場合 → 傾斜に沿って加速
		slopeDir = slopeDir.Normalize();
		Vector3 slopeAccel = slopeDir * slopeGravity_ * slopeLen;

		// 現在速度に加速度を加える
		currentVelo_ += slopeAccel;
	}
	else {
		//傾きがない場合減速
		float spd = currentVelo_.Length();
		
		//速度が減速量より多い場合
		if(spd>decelRate){
			//減速したベクトルを計算
			Vector3 decAccel = currentVelo_.Normalize() * (spd - decelRate);

			//渡す
			currentVelo_ = decAccel;
		}
		else{
			//速度を0にする
			currentVelo_ = Vector3{0,0,0};
		}
	}

	//移動する
	player_->parameters_.velocity += currentVelo_;


}
