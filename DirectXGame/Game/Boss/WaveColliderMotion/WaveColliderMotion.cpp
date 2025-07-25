#include"WaveColliderMotion.h"
#include"ColliderManager.h"
#include"ShapesDraw.h"

WaveColliderMotion::WaveColliderMotion()
{
	collider_ = std::make_unique<DaiEngine::CylinderCollider>();
	collider_->Init("wave", world_, radius_,height_);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider* collider) {OnCollision(collider); });
	collider_->ColliderOff();

	tree_.name_ = "WaveColliuder";
	tree_.SetMonitorValue("isDead", &isEnd_);
	tree_.SetMonitorValue("count", &count_);
	tree_.SetMonitorValue("radius", &radius_);
	tree_.SetValue("addRadius", &addRadius_);
	tree_.SetValue("maxCount", &maxCount_);
	tree_.SetValue("height", &height_);
}

WaveColliderMotion::~WaveColliderMotion()
{
	DaiEngine::ColliderManager::GetInstance()->RemoveCollider(collider_.get());
}

void WaveColliderMotion::Emit(const Vector3& center)
{
	isEnd_ = false;
	world_.translation_ = center;
	radius_ = 0;
	count_ = 0;
	collider_->ColliderOn();
}

void WaveColliderMotion::Update() {

	//終了フラグが立っていない場合
	if (!isEnd_) {

		if (count_++ >= maxCount_) {
			isEnd_ = true;
		}

		//サイズを変更
		radius_ += addRadius_;
		collider_->SetRadius(radius_);
		collider_->SetHeight(height_);

		Matrix4x4 rotateM = MakeRotateXMatrix(0) * MakeRotateYMatrix(0) * MakeRotateZMatrix(0);
		collider_->Update(rotateM);
	}
}

void WaveColliderMotion::Draw(DaiEngine::Camera* camera) {
#ifdef _DEBUG
	if (isEnd_)return;
	ShapesDraw::DrawCylinder(std::get<Shapes::Cylinder>(collider_->GetShape()), *camera, { 1,1,1,1 });
#endif // _DEBUG

}

void WaveColliderMotion::OnCollision(DaiEngine::Collider* collider) {
	if (collider->GetTag() == "player") { 
		collider_->ColliderOff();
		isEnd_ = true;
	}
}