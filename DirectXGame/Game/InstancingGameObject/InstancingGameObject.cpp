#include "InstancingGameObject.h"
#include"ModelManager.h"

//カメラポインタ初期化
DaiEngine::Camera* InstancingGameObject::camera_ = nullptr;

void InstancingGameObject::Init(const std::string& modelName, int drawNum)
{
	model_.reset(DaiEngine::InstancingObject3d::Create(DaiEngine::ModelManager::LoadGLTF(modelName), drawNum));
}

void InstancingGameObject::Update()
{
	//セットされたデータ更新
	for (auto& data : datas_) {
		//行列更新
		data.worldTransform_.UpdateMatrix();
	}
}

void InstancingGameObject::Draw()
{
	//描画
	model_->Draw(*camera_,datas_);

	//データクリア
	datas_.clear();
}
