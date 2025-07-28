#include "BossSpinAttack.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"
#include "EffectManager.h"
#include "AudioManager.h"

Boss2SpinAttack::Boss2SpinAttack(BossParameters* param)
{
	param_ = param;
	dangerZone_ = std::make_unique<SquareDangerZone>(param->world);

	//コライダー生成
	collider_ = std::make_unique<DaiEngine::SphereCollider>();
	collider_->Init("bossCharge",*param_->world , radius_);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });
	collider_->ColliderOff();

	tree_.name_ = "SpinAttack2Player";

	tree_.SetMonitorValue("currentExecuteCount", &currentExecuteCount_);

	tree_.SetValue("downCount", &downCount_);
	tree_.SetValue("executeCount", &executeCount_);
	tree_.SetValue("ptrAnimationRate", &preActionRate_);
	tree_.SetValue("actionAnimationRate", &actionRate_);
	tree_.SetValue("downActionRate", &downActionRate_);
	tree_.SetValue("speed", &speed_);
	tree_.SetValue("radius", &radius_);
	tree_.SetValue("color", &color_);
	tree_.SetTreeData(dangerZone_->GetTree());

	dashSE_ = DaiEngine::AudioManager::Load("SE/DonutDash.mp3");
}

Boss2SpinAttack::~Boss2SpinAttack()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void Boss2SpinAttack::Draw()
{
#ifdef _DEBUG
	ShapesDraw::DrawSphere(std::get<Shapes::Sphere>(collider_->GetShape()), *camera_, color_);
#endif // _DEBUG

	if (isDrawZone_) {
		dangerZone_->Draw();
	}

}

void Boss2SpinAttack::InitBehavior0()
{
	//アニメーション変更
	param_->setAnimeName_ = "Donut_AttackStandby";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = preActionRate_;
	param_->currentSec = 0;
	collider_->ColliderOff();

	dangerZone_->Init();
	isDrawZone_ = true;
	param_->cameraBehaviorRequest_ = FollowCamera::State::None;
}

void Boss2SpinAttack::InitBehavior1()
{
	//アニメーション変更
	param_->setAnimeName_ = "Donut_Attack";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = actionRate_;

	//速度計算
	velo_ = param_->lookAtPlayerVec_.Normalize() * speed_;
	param_->isHitMapEdge_ = false;
	collider_->ColliderOn();
	collider_->SetRadius(radius_);

	emitPos_ = collider_->GetWorldPos() + Vector3(0.0f, 2.0f, 0.0f);
	EffectManager::GetInstance()->Start("DonutsRollEffect", &emitPos_);
	isDrawZone_ = false;

	dashSE_->Play();
}

void Boss2SpinAttack::InitBehavior2()
{
	//アニメーション変更
	param_->setAnimeName_ = "Donut_AttackEnd";
	param_->isLoopAnime_ = false;
	//再生速度変更
	param_->animationLeverage_ = downActionRate_;
	param_->cameraBehaviorRequest_ = FollowCamera::State::Follow;
}

void Boss2SpinAttack::UpdateBehavior0()
{
	dangerZone_->Update();
	//プレイヤー方向を向く
	param_->isLookAtPlayer_ = true;
	if (dangerZone_->isEnd()) {	
		countRequest_ = 1;	
	}
}

void Boss2SpinAttack::UpdateBehavior1()
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
		EffectManager::GetInstance()->End("DonutsRollEffect");
	}

	collider_->Update();
	emitPos_ = collider_->GetWorldPos() + Vector3(0.0f, 2.0f, 0.0f);
}

void Boss2SpinAttack::UpdateBehavior2()
{
	if(param_->currentSec >= downCount_) {
		//終了
		param_->behaviorRequest_ = 0;
	}
}

void Boss2SpinAttack::OnCollision(DaiEngine::Collider* collider)
{
	//プレイヤーに当たったらoff
	if (collider->GetTag() == "player") {
		collider_->ColliderOff();
	}
}