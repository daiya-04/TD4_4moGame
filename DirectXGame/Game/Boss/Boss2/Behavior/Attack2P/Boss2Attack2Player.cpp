#include "Boss2Attack2Player.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

Boss2Attack2Player::Boss2Attack2Player()
{
	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("bossCharge",*param_->world , radius_);
	collider_->ColliderOn();
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });
	collider_->ColliderOff();

	tree_.name_ = "SpinAttack2Player";

	tree_.SetMonitorValue("currentExecuteCount", &currentExecuteCount_);

	tree_.SetValue("waitCount", &waitTime_);
	tree_.SetValue("executeCount", &executeCount_);
	tree_.SetValue("ptrAnimationRate", &preActionRate_);
	tree_.SetValue("actionAnimationRate", &actionRate_);
	tree_.SetValue("downActionRate", &downActionRate_);
	tree_.SetValue("speed", &speed_);
	tree_.SetValue("radius", &radius_);
	tree_.SetValue("color", &color_);
}

Boss2Attack2Player::~Boss2Attack2Player()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());

}

void Boss2Attack2Player::Draw()
{
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, color_);
#endif // _DEBUG
}

void Boss2Attack2Player::InitBehavior0()
{
	//アニメーション変更
	param_->setAnimeName_ = "DonutAttack2";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = preActionRate_;
	param_->currentSec = 0;
	collider_->ColliderOff();
}

void Boss2Attack2Player::InitBehavior1()
{
	//アニメーション変更
	param_->setAnimeName_ = "DonutAttack2";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = actionRate_;

	//速度計算
	velo_ = param_->lookAtPlayerVec_.Normalize() * speed_;
	param_->isHitMapEdge_ = false;
	collider_->ColliderOn();
}

void Boss2Attack2Player::InitBehavior2()
{
	//アニメーション変更
	param_->setAnimeName_ = "DonutDown";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = downActionRate_;
}

void Boss2Attack2Player::UpdateBehavior0()
{
	//プレイヤー方向を向く
	param_->isLookAtPlayer_ = true;
	if (param_->currentSec >= waitTime_) {	
		countRequest_ = 1;	
	}
}

void Boss2Attack2Player::UpdateBehavior1()
{

	//veloを渡し続ける
	param_->velocity_ = velo_;

	//ボスがマップ端に到達
	if (param_->isHitMapEdge_) {
		param_->isHitMapEdge_ = false;
		//回数カウント
		currentExecuteCount_++;
		//タックル回数が指定未満なら繰り返す
		if (currentExecuteCount_ <= executeCount_) {	
			//前状態リクエスト
			countRequest_ = 0;
			param_->currentSec = 0.0f; //経過時間リセット
		}
		else {//

			//タックル回数のリセット
			currentExecuteCount_ = 0;
			//通常状態に移行
			countRequest_ = 2;
		}
	}

	collider_->Update();
}

void Boss2Attack2Player::UpdateBehavior2()
{
	if(param_->currentSec >= downCount_) {
		//終了
		param_->behaviorRequest_ = 0;
	}
}

void Boss2Attack2Player::OnCollision(DaiEngine::Collider* collider)
{
	//プレイヤーに当たったらoff
	if (collider->GetTag() == "player") {
		collider_->ColliderOff();
	}
}