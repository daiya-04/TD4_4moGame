#include "GameObject.h"
#include"ModelManager.h"

//初期化
DaiEngine::Camera* GameObject::camera_ = nullptr;

void GameObject::Init(const std::string& modelName)
{
	//モデルクラスの初期化
	model_.reset(DaiEngine::SkinningObject::Create(DaiEngine::ModelManager::LoadGLTF(modelName)));

	world_ = &model_->worldTransform_;
}

void GameObject::Update()
{
	//行列更新
	model_->Update();
}

void GameObject::Draw()
{
	//体描画
	model_->Draw(*camera_);
}

void GameObject::SetAnimationName(const std::string& name, bool isLoop)
{
	//アニメーション名を設定
	model_->SetAnimation(name,isLoop);
}

void GameObject::SetAnimationLeverage(float leverage)
{
	//倍率変更
	model_->GetAnimation().SetAnimationSpeed(leverage);
}

