#include "SingleDangerZone.h"
#include"Boss/DangerZone/Zones/Simple/SinpleDangerZone.h"


SingleDangerZone::SingleDangerZone(const DangerZoneParameters& param)
{
	//警告円生成
	dangerZone_ = std::make_unique<SinpleDangerZone>(param);
	dangerZone_->SetPlayerPos(&pPos_);
	//オブジェクト生成
	gameObject_ = std::make_unique<InstancingGameObject>();
	gameObject_->Init("DangerZone",10);

}

void SingleDangerZone::Init(const DangerZoneParameters& param)
{
	dangerZone_ = std::make_unique<SinpleDangerZone>(param);
	dangerZone_->Reset();
	isDraw_ = true;
}

void SingleDangerZone::Update()
{
	//警告円更新
	dangerZone_->Update();
	isDraw_ = dangerZone_->GetIsDraw();


	DaiEngine::InstancingObjData data;
	data.worldTransform_ = dangerZone_->GetWorld();
	data.color_ = *color_;
	data.alpha_ = color_->w;

	gameObject_->SetData(data);



	gameObject_->Update();
}

void SingleDangerZone::Draw()
{
	//警告円描画
	if (isDraw_) {
		gameObject_->Draw();
	}
}