#include "SingleDangerZone.h"

SingleDangerZone::SingleDangerZone(const DangerZoneParameters& param)
{
	//警告円生成
	dangerZone_ = std::make_unique<DangerZone>(param);
	//オブジェクト生成
	gameObject_ = std::make_unique<GameObject>();
	gameObject_->Init("DangerZone");


}

void SingleDangerZone::Init(const DangerZoneParameters& param)
{
	dangerZone_->SetParameter(param);
	dangerZone_->Reset();
	isDraw_ = true;
}

void SingleDangerZone::Update()
{
	//警告円更新
	dangerZone_->Update();
	gameObject_->world_->translation_ = dangerZone_->GetWorld().translation_;
	gameObject_->world_->scale_ = dangerZone_->GetWorld().scale_;
	gameObject_->world_->UpdateMatrix();
	isDraw_ = dangerZone_->GetIsDraw();
}

void SingleDangerZone::Draw()
{
	//警告円描画
	if (isDraw_) {
		gameObject_->Draw();
	}
}