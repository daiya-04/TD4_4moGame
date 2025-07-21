#include "GameObject.h"
#include"ModelManager.h"

//初期化
DaiEngine::Camera* GameObject::camera_ = nullptr;

void GameObject::Init(const std::string& modelName,LoadType type)
{
	//モデルクラスの初期化
	if (type == LoadType::Gltf) {
		model_.reset(DaiEngine::SkinningObject::Create(DaiEngine::ModelManager::LoadGLTF(modelName)));
	}else if(type == LoadType::Obj) {
		model_.reset(DaiEngine::SkinningObject::Create(DaiEngine::ModelManager::LoadOBJ(modelName)));
	}

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

#ifdef _DEBUG
	// スケルトン描画（Debugのみ）
	model_->GetSkeleton().Draw(model_->worldTransform_, *camera_);
#endif
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

Vector3 GameObject::GetJointWorldPosition(const std::string& jointName) {
	// スケルトンからローカル空間の位置を取得
	Vector3 localPos = model_->GetSkeleton().GetSkeletonPos(jointName);

	// ワールド変換を適用（位置ベクトルなので第4引数は1）
	Vector3 worldPos = Transform(localPos, world_->matWorld_);

	return worldPos;
}
