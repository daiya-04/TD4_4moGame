#include "GameObject.h"
#include"ModelManager.h"

//初期化
DaiEngine::Camera* GameObject::camera_ = nullptr;

void GameObject::Init(const std::string& modelName)
{
	//モデルクラスの初期化
	model_.reset(DaiEngine::SkinningObject::Create(DaiEngine::ModelManager::LoadGLTF(modelName)));

	//スケルトン生成
	skeleton_ = DaiEngine::Skeleton::Create(model_.get()->GetModel()->rootNode_);

	//スキンクラスター生成
	skinCluster_ = std::make_unique<DaiEngine::SkinCluster>();
	skinCluster_->Create(skeleton_, model_.get()->GetModel());

	//スキンクラスターをモデルにセット
	model_->SetSkinCluster(skinCluster_.get());

	world_ = &model_->worldTransform_;
}

void GameObject::Update()
{
	//行列更新
	model_->worldTransform_.UpdateMatrix();
	//スケルトンの更新
	skeleton_.Update();
	//スキンクラスターの更新
	skinCluster_->Update(skeleton_);
}

void GameObject::Draw()
{
	//体描画
	model_->Draw(*camera_);
}

